//
// Created by benja on 22/06/2023.
//
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "renderer.h"
#include "GameObject.h"
#include "Game.h"
#include "helper.h"
#include "deltaTime.h"
#include "BehaviourComponent.h"


int gameMap[10][10]{
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,3,0,5,0,2,0,1},
        {1,0,2,0,4,0,0,2,0,1},
        {1,0,0,0,2,0,3,0,5,1},
        {1,0,4,3,5,0,2,0,0,1},
        {1,0,2,2,2,0,4,0,0,1},
        {1,0,2,0,3,0,0,2,0,1},
        {1,0,4,0,2,0,3,0,4,1},
        {1,0,2,3,0,2,0,2,0,1},
        {1,1,1,1,1,1,1,1,1,1}
};

GameObject *createTarget(Game &game, float x, float y, float z) {
    GameObject* target = game.InstantiateGameObjectBeforeStart("target", glm::vec3(x,y,z));
    target->addComponent(new DisappearOnHitBehaviour(), &game);
    game.createRenderInfo(*target, 2, 1);
    CircleBound* bound = target->addCirclebound(&game);
    return target;
}

Game::Game(Renderer* renderer) : renderer{renderer} { rootNode = new GameObject(); rootNode->isRoot = true; rootNode->id = "ROOT";}
Game::~Game() { delete rootNode;}

GameObject* Game::InstantiateGameObjectBeforeStart(const glm::vec3 &pos) {
    auto test = new GameObject(pos);
    rootNode->addChild(test);
    return test;
}

GameObject* Game::InstantiateGameObjectBeforeStart(std::string& id, const glm::vec3 &pos) {
    auto test = new GameObject(id, pos);
    rootNode->addChild(test);
    return test;
}
GameObject* Game::InstantiateGameObjectBeforeStart(std::string id, const glm::vec3 &pos) {
    auto test = new GameObject(std::move(id), pos);
    rootNode->addChild(test);
    return test;
}

GameObject* Game::InstantiateGameObject(std::string& id, const glm::vec3 &pos) {
    auto test = new GameObject(id, pos);
    rootNode->addChild(test);
    test->start(frameData);
    return test;
}
GameObject* Game::InstantiateGameObject(std::string id, const glm::vec3 &pos) {
    auto test = new GameObject(std::move(id), pos);
    rootNode->addChild(test);
    test->start(frameData);
    return test;
}



void Game::mainLoop() {
    float physicsTimer = 0;
    bool firstFrame{true};
    while (!glfwWindowShouldClose(renderer->window)) {
        //if(checkoutRound)println("begin Frame");
        glfwPollEvents();
        if (firstFrame && !rootNode->getChildren().empty()) {
            for(auto& child : rootNode->getChildren()){
                std::function<void(GameObject*)> start_lambda = [&](GameObject* a) {a->start(frameData);};
                GameObject::do_for_all_nodes(child, start_lambda);
            }
        }
        //if(checkoutRound)println("start done");
        firstFrame = false;
        for(auto& child : rootNode->getChildren()){
            std::function<void(GameObject*)> update_lambda = [&](GameObject* a) {a->update(frameData);};
            GameObject::do_for_all_nodes(child, update_lambda);
        }
        //if(checkoutRound)println("update done");
        physicsTimer += deltaTime();
        resetFrameTime();
        if(physicsTimer > 0.02f) {
            //if(checkoutRound)println("physics start");
            check_hitboxes();
            //if(checkoutRound)println("physics done");
            physicsTimer = 0;
        }
        renderer->drawFrame(*rootNode, frameData.camera);
        //if(checkoutRound)println("drawFrame", deltaTime());
        destroyScheduledGameObjects();
        //if(checkoutRound)println("destroy", deltaTime());
        // if(checkoutRound)checkoutRound--;
        cycle++;

    }
    vkDeviceWaitIdle(renderer->device);
}

void Game::check_hitboxes(){
    for (int currentBound{0}; currentBound < circleBounds.size(); currentBound++) {
        for (int nextBound{currentBound + 1}; nextBound < circleBounds.size(); nextBound++) {
            if (CircleBound::circle_circle(circleBounds[currentBound], circleBounds[nextBound])) {
                auto text {circleBounds[currentBound]->gameObject->id + " hit " +circleBounds[nextBound]->gameObject->id};
                circleBounds[currentBound]->gameObject->onHit(frameData, circleBounds[nextBound]->gameObject->is_static());
                circleBounds[nextBound]->gameObject->onHit(frameData, circleBounds[currentBound]->gameObject->is_static());
                //println(text);
                if(!circleBounds[currentBound]->trigger && !circleBounds[nextBound]->trigger){
                    if(circleBounds[currentBound]->gameObject->is_static()){
                        auto pos1 = circleBounds[currentBound]->getWorldPoint();
                        auto rad1 = circleBounds[currentBound]->radius;
                        auto pos2 = circleBounds[nextBound]->getWorldPoint();
                        auto rad2 = circleBounds[nextBound]->radius;
                        float distance = glm::distance(pos1,pos2);
                        glm::vec3 dir = glm::normalize(pos2 - pos1);
                        circleBounds[nextBound]->gameObject->move(dir * ((rad1+rad2) - distance));
                    }
                    else{
                        auto pos1 = circleBounds[nextBound]->getWorldPoint();
                        auto rad1 = circleBounds[nextBound]->radius;
                        auto pos2 = circleBounds[currentBound]->getWorldPoint();
                        auto rad2 = circleBounds[currentBound]->radius;
                        float distance = glm::distance(pos1,pos2);
                        glm::vec3 dir = glm::normalize(pos2 - pos1);
                        circleBounds[currentBound]->gameObject->move(dir * ((rad1+rad2)- distance));
                    }
                }

            }
        }
    }
}

void Game::destroyScheduledGameObjects(){
    if(removalQueue.empty()) return;
    checkoutRound = 2;
    //println("before CLEANUP");
    for(GameObject* obj : removalQueue){
        if(obj == nullptr) throw std::runtime_error("HOW DID IT GET HERE?");
        //println("deleted ", obj->id);
        cleanupSingleGameObjectRenderInfo(obj);
        //println("before circleBOunds", obj->id);
        if(obj->circleBound){
            for(auto it{circleBounds.begin()}; it != circleBounds.end(); ++it){
                if(*it == obj->circleBound){
                    //println("DELETED BOUND");
                    circleBounds.erase(it);
                    break;
                }
            }
        }
        //println("before cubeBounds", obj->id);
        if(obj->cubeBound){
            for(auto it{cubeBounds.begin()}; it != cubeBounds.end(); ++it){
                if(*it == obj->cubeBound){
                    //println("DELETED BOUND");
                    cubeBounds.erase(it);
                    break;
                }
            }
        }
        //println("before player", obj->id);
        if (this->player == obj){
            player = nullptr;
        }
        //println("delete", obj->id);
        delete obj;
        //println("deleteDOne");
    }
    removalQueue.clear();
    //println("didn't crash in CLEANUP");
}

void Game::cleanupAllGameObjectRenderMemory() const{
    std::function<void(GameObject*)> deleteLambda = [&](GameObject* obj){
        cleanupSingleGameObjectRenderInfo(obj);
    };
    GameObject::do_for_all_nodes(rootNode, deleteLambda);
}

void Game::cleanupSingleGameObjectRenderInfo(GameObject * obj) const{
    if(obj->renderInfo){
        renderer->addRenderInfo(obj->renderInfo);
        obj->renderInfo = nullptr;
    }
}

void Game::scheduleGameObjectRemoval(GameObject * obj){
    //println("round", cycle);
    //println("QUEUED", obj->id);
    std::function<void(GameObject*)> queueRemoval = [&](GameObject* obj) {
        for(auto it{removalQueue.begin()}; it != removalQueue.end(); ++it){
            if(*it == obj){
                return;
            }
        }
        removalQueue.push_back(obj);
    };
    GameObject::do_for_all_nodes(obj, queueRemoval);
    if(obj->parent)obj->parent->removeChild(obj);
    obj->parent = nullptr;
}

void Game::createRenderInfo(GameObject& obj, uint32_t textureIndex, uint32_t modelIndex) const {
    if(obj.renderInfo) {
        throw std::runtime_error("creating RenderInfo on a gameObject that already has one");
    }
    obj.renderInfo = renderer->getRenderInfo(textureIndex, modelIndex);
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        Game::keymap[key] = 1;
    }
    else if(action == GLFW_RELEASE){
        Game::keymap[key] = -1;
    }
}


void spawnMap(Game &game, float scaleX, float scaleZ, int row, int col) {
    switch(gameMap[row][col]){
        case 1:{
            GameObject* obst = game.InstantiateGameObjectBeforeStart("obst" + std::to_string(row) + "," + std::to_string(col), glm::vec3(row*scaleX/10,2.5,col*scaleZ/10));
            obst->scale = glm::vec3(scaleX/10,5,scaleZ/10);
            game.createRenderInfo(*obst, 1, 2);
            obst->set_static(true);
            obst->id = "obst1" + std::to_string(row) + ":" + std::to_string(col);
            break;
        }
        case 2:{
            GameObject* obst = game.InstantiateGameObjectBeforeStart("obst" + std::to_string(row) + "," + std::to_string(col), glm::vec3(row*scaleX/10,0,col*scaleZ/10));
            auto size = (row+col+1.0f)/6.0f;
            obst->scale = glm::vec3(2 *size);
            CircleBound* hitbox = obst->addCirclebound(&game);
            hitbox->radius = size;
            game.createRenderInfo(*obst, 1, 1);
            obst->set_static(true);
            obst->id = "obst2" + std::to_string(row) + ":" + std::to_string(col);
            break;
        }
        case 3:{
            GameObject *target = createTarget(game, row * scaleX / 10, 2, col * scaleZ / 10);
            target->addComponent(new BackAndForthBehaviour(target->position, target->position + glm::vec3((row%3), 1, (col%2)), (row%3) + col%3 * 0.2f), &game);
            target->addComponent(new EnemyTargetBehaviour(5 + (row%2) * 0.3f + (col%2) * 0.2f), &game);
            target->id = "target3" + std::to_string(row) + ":" + std::to_string(col);
            break;
        }

        case 4:{
            GameObject* testEnemy = createTarget(game, row*scaleX/10, 3,  col*scaleZ/10);
            testEnemy->addComponent(new BackAndForthBehaviour(testEnemy->position, testEnemy->position + glm::vec3((row%2) * 0.3f, 3, (col%3) * 0.3f), (row%3)*0.2f + col%3 * 0.3f), &game);
            testEnemy->addComponent(new EnemyTargetBehaviour(5 + (row%3) * 0.3f - (col%2) * 0.2f), &game);
            testEnemy->id = "testEnemy4" + std::to_string(row) + ":" + std::to_string(col);
            break;
        }
        case 5:{
            GameObject *target = createTarget(game, row * scaleX / 10, 2, col * scaleZ / 10);
            target->addComponent(new BackAndForthBehaviour(target->position, target->position + glm::vec3((row%2) * 0.3f, 1, (col%2) * 0.3f), (row%3)*0.3f + col%3 * 0.4f), &game);
            target->addComponent(new EnemyTargetBehaviour(5 - (row%2) * 0.3f + (col%3) * 0.2f), &game);
            target->id = "target3" + std::to_string(row) + ":" + std::to_string(col);
            break;
        }
        default:{}
    }
}

int main() {
    Renderer renderer;
    Game game{&renderer};


    try {
        //setup renderer
        renderer.model_paths = {"viking_room.obj", "sphere.obj", "cube.obj", "tank.obj", "map.obj"};
        renderer.texture_paths = {"viking_room.png", "white.jpeg", "red.jpeg", "green.jpeg", "blue.jpeg", "test.png"};
        renderer.init();
        glfwSetKeyCallback(renderer.window, Game::key_callback);

        //tank setup
        float scaleX = 50, scaleZ = 50;
        GameObject* tank = game.InstantiateGameObjectBeforeStart("tank", glm::vec3(scaleX/2,0,scaleZ/2));
        GameObject* tankhead = game.InstantiateGameObjectBeforeStart("tankhead", glm::vec3(0,0.2f,0));
        GameObject* muzzle = game.InstantiateGameObjectBeforeStart("muzzle", glm::vec3(0,0,0));
        GameObject* tankheadVisual = game.InstantiateGameObjectBeforeStart("tankhead_vis", glm::vec3(0,0,0));

        tank->addCirclebound(&game);
        tank->getCirclebound()->radius = 0.4f;
        tankhead->setParent(tank);
        tankheadVisual->setParent(tankhead);
        muzzle->setParent(tankhead);
        tankheadVisual->scale = glm::vec3(0.5f);
        game.player = tank;
        game.createRenderInfo(*tank, 2, 3);
        game.createRenderInfo(*tankheadVisual, 3, 3);
        tank->addComponent(new MovementBehaviour(), &game);
        tank->addComponent(new HealthBehaviour(), &game);
        tankhead->addComponent(new RotationMovementBehaviour(), &game);
        muzzle->addComponent(new ShootBehaviour(), &game);
        muzzle->addComponent(new CamFollowBehaviour(), &game);

        Camera& cam = game.frameData.camera;
        cam.position = glm::vec3(0,5,5);
        cam.target = glm::vec3(0);
        cam.up = glm::vec3(0,1,0);

        GameObject* ground = game.InstantiateGameObjectBeforeStart("Ground", glm::vec3(scaleX/2,0,scaleZ/2));
        ground->scale = glm::vec3(scaleX,0,scaleZ);
        game.createRenderInfo(*ground, 3, 2);

        for(int row{0}; row < 10; row++){
            for(int col{0}; col < 10; col++){
                spawnMap(game, scaleX, scaleZ, row, col);
            }
        }

        //TEST






        //start game loop
        resetGameTime();
        resetFrameTime();
        game.mainLoop();

        //cleanup
        game.cleanupAllGameObjectRenderMemory();
        renderer.cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    _CrtDumpMemoryLeaks();
    return EXIT_SUCCESS;
}




void println(glm::vec3 vector){
    std::cout << glm::to_string(vector) << "\n";
}

void println(glm::mat4 mat) {
    std::cout << glm::to_string(mat) << "\n";
}

void println(std::string str, glm::mat4 mat) {
    std::cout << str << " " << glm::to_string(mat) << "\n";
}

void println(std::string str, glm::vec3 vector) {
    std::cout << str << " " << glm::to_string(vector) << "\n";
}
