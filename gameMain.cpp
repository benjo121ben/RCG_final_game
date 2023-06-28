//
// Created by benja on 22/06/2023.
//

#include "renderer.h"
#include "GameObject.h"
#include "Game.h"
#include "helper.h"
#include "deltaTime.h"
#include "BehaviourComponent.h"

int gameMap[10][10]{
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,2,0,1},
        {1,0,2,0,0,0,0,2,0,1},
        {1,0,0,0,2,0,0,0,0,1},
        {1,0,0,0,0,0,2,0,0,1},
        {1,0,2,2,2,0,0,0,0,1},
        {1,0,2,0,0,0,0,2,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,2,0,2,0,1},
        {1,1,1,1,1,1,1,1,1,1}
};

GameObject *createTarget(Game &game, float x, float y, float z) {
    GameObject* target = game.InstantiateGameObjectBeforeStart("target", glm::vec3(x,y,z));
    target->addComponent(new DisappearOnHitBehaviour(), &game);
    game.createRenderInfo(*target, 2, 1);
    CircleBound* bound = target->addCirclebound(&game);
    return target;
}

Game::Game(Renderer* renderer) : renderer{renderer} { rootNode = new GameObject(); rootNode->isRoot = true;}
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
        glfwPollEvents();
        if (firstFrame && !rootNode->getChildren().empty()) {
            for(auto& child : rootNode->getChildren()){
                std::function<void(GameObject*)> start_lambda = [&](GameObject* a) {a->start(frameData);};
                GameObject::do_for_all_nodes(child, start_lambda);
            }
        }
        firstFrame = false;
        for(auto& child : rootNode->getChildren()){
            std::function<void(GameObject*)> update_lambda = [&](GameObject* a) {a->update(frameData);};
            GameObject::do_for_all_nodes(child, update_lambda);
        }
        physicsTimer += deltaTime();
        if(physicsTimer > 0.02f) {
            check_hitboxes();
        }
        resetFrameTime();
        renderer->drawFrame(*rootNode, frameData.camera);
        destroyScheduledGameObjects();
    }
    vkDeviceWaitIdle(renderer->device);
}

void Game::check_hitboxes(){
    for (int currentBound{0}; currentBound < circleBounds.size(); currentBound++) {
        for (int nextBound{currentBound + 1}; nextBound < circleBounds.size(); nextBound++) {
            if (CircleBound::circle_circle(circleBounds[currentBound], circleBounds[nextBound])) {
                circleBounds[currentBound]->gameObject->onHit(frameData);
                circleBounds[nextBound]->gameObject->onHit(frameData);
            }
        }
    }
}

void Game::destroyScheduledGameObjects(){
    while(!removalQueue.empty()){
        auto obj = removalQueue.front();
        println("deleted ", obj->id);
        removalQueue.pop();
        if(obj->renderInfo){
            renderer->cleanupRenderInfo(obj->renderInfo);
            delete obj->renderInfo;
            obj->renderInfo = nullptr;
        }
        if(obj->test){
            for(auto it{circleBounds.begin()}; it != circleBounds.end(); ++it){
                if(*it == obj->test){
                    println("DELETED BOUND");
                    circleBounds.erase(it);
                    return;
                }
            }
        }
        if(obj->test2){
            for(auto it{cubeBounds.begin()}; it != cubeBounds.end(); ++it){
                if(*it == obj->test2){
                    println("DELETED BOUND");
                    cubeBounds.erase(it);
                    return;
                }
            }
        }
        delete obj;
    }
}

void Game::cleanupGameObjectRenderMemory(){
    std::function<void(GameObject*)> deleteLambda = [&](GameObject* obj){
        if(obj->renderInfo){
            renderer->cleanupRenderInfo(obj->renderInfo);
            delete obj->renderInfo;
            obj->renderInfo = nullptr;
        }
    };
    for(auto& obj : rootNode->getChildren()) {
        GameObject::do_for_all_nodes(obj, deleteLambda);
    }
}

void Game::scheduleGameObjectRemoval(GameObject * obj){
    removalQueue.push(obj);
}

void Game::createRenderInfo(GameObject& obj, uint32_t textureIndex, uint32_t modelIndex) {
    if(obj.renderInfo) {
        throw std::runtime_error("creating RenderInfo on a gameObject that already has one");
    }
    obj.renderInfo = new RenderInfo;
    renderer->createRenderInfo(obj.renderInfo, textureIndex, modelIndex);
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
        tankhead->setParent(tank);
        tankheadVisual->setParent(tankhead);
        muzzle->setParent(tankhead);
        tankheadVisual->scale = glm::vec3(0.5f);

        //targets
        createTarget(game, scaleX/2, 1,  scaleZ/2-2);
        GameObject *target2 = createTarget(game, scaleX/2+3.5f, 1,  scaleZ/2-2);
        target2->addComponent(new BackAndForthBehaviour(), &game);

        Camera& cam = game.frameData.camera;
        cam.position = glm::vec3(0,5,5);
        cam.target = glm::vec3(0);
        cam.up = glm::vec3(0,1,0);

        GameObject* ground = game.InstantiateGameObjectBeforeStart("Ground", glm::vec3(scaleX/2,0,scaleZ/2));
        ground->scale = glm::vec3(scaleX,0,scaleZ);
        game.createRenderInfo(*ground, 3, 2);

        for(int row{0}; row < 10; row++){
            for(int col{0}; col < 10; col++){
                if(gameMap[row][col] == 1){
                    GameObject* obst = game.InstantiateGameObjectBeforeStart("obst" + std::to_string(row) + "," + std::to_string(col), glm::vec3(row*scaleX/10,2.5,col*scaleZ/10));
                    obst->scale = glm::vec3(scaleX/10,5,scaleZ/10);
                    game.createRenderInfo(*obst, 1, 2);
                }

                if(gameMap[row][col] == 2){
                    GameObject* obst = game.InstantiateGameObjectBeforeStart("obst" + std::to_string(row) + "," + std::to_string(col), glm::vec3(row*scaleX/10,0,col*scaleZ/10));
                    obst->scale = glm::vec3(1.2f,1.3f,1.6f);
                    game.createRenderInfo(*obst, 1, 2);
                }
            }
        }

        //TEST

        // map->setRotation(glm::vec3(-90.0f, 0.0f, -90.0f));
        //game.createRenderInfo(*map, 1, 4);
        game.createRenderInfo(*tank, 2, 3);
        game.createRenderInfo(*tankheadVisual, 3, 3);
        tank->addComponent(new MovementBehaviour(), &game);
        tankhead->addComponent(new RotationMovementBehaviour(), &game);
        muzzle->addComponent(new ShootBehaviour(), &game);
        muzzle->addComponent(new CamFollowBehaviour(), &game);


        //start game loop
        resetGameTime();
        resetFrameTime();
        game.mainLoop();

        //cleanup
        game.cleanupGameObjectRenderMemory();
        renderer.cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

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
