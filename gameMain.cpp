//
// Created by benja on 22/06/2023.
//

#include "renderer.h"
#include "GameObject.h"
#include "Game.h"
#include "helper.h"
#include "deltaTime.h"


Game::Game(Renderer* renderer) : renderer{renderer} {}

GameObject* Game::InstantiateGameObjectBeforeStart(const glm::vec3 &pos) {
    auto test = new GameObject(pos);
    rootNode.addChild(test);
    return test;
}

GameObject* Game::InstantiateGameObjectBeforeStart(std::string& id, const glm::vec3 &pos) {
    auto test = new GameObject(id, pos);
    rootNode.addChild(test);
    return test;
}
GameObject* Game::InstantiateGameObjectBeforeStart(std::string id, const glm::vec3 &pos) {
    auto test = new GameObject(std::move(id), pos);
    rootNode.addChild(test);
    return test;
}

GameObject* Game::InstantiateGameObject(std::string& id, const glm::vec3 &pos) {
    auto test = new GameObject(id, pos);
    rootNode.addChild(test);
    test->start(frameData);
    return test;
}
GameObject* Game::InstantiateGameObject(std::string id, const glm::vec3 &pos) {
    auto test = new GameObject(std::move(id), pos);
    rootNode.addChild(test);
    test->start(frameData);
    return test;
}



void Game::mainLoop() {
    bool firstFrame{true};
    while (!glfwWindowShouldClose(renderer->window)) {
        glfwPollEvents();
        if (firstFrame && !rootNode.getChildren().empty()) {
            for(auto& child : rootNode.getChildren()){
                std::function<void(GameObject*)> start_lambda = [&](GameObject* a) {a->start(frameData);};
                GameObject::do_for_all_nodes(child, start_lambda);
            }
        }
        firstFrame = false;
        for(auto& child : rootNode.getChildren()){
            std::function<void(GameObject*)> update_lambda = [&](GameObject* a) {a->update(frameData);};
            GameObject::do_for_all_nodes(child, update_lambda);
        }
        resetFrameTime();
        renderer->drawFrame(rootNode, frameData.camera);
        destroyScheduledGameObjects();
    }
    vkDeviceWaitIdle(renderer->device);
}

void Game::destroyScheduledGameObjects(){
    while(!removalQueue.empty()){
        auto obj = removalQueue.front();
        removalQueue.pop();
        if(obj->renderInfo){
            renderer->cleanupRenderInfo(obj->renderInfo);
            delete obj->renderInfo;
            obj->renderInfo = nullptr;
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
    for(auto& obj : rootNode.getChildren()) {
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



struct test{
    std::string a;
};

int main() {
    Renderer renderer;
    Game game{&renderer};


    try {
        //setup
        renderer.model_paths = {"viking_room.obj", "sphere.obj", "cube.obj", "tank.obj", "map.obj"};
        renderer.texture_paths = {"viking_room.png", "white.jpeg", "red.jpeg", "green.jpeg", "blue.jpeg", "test.png"};
        renderer.init();
        glfwSetKeyCallback(renderer.window, Game::key_callback);

        //obj setup
        GameObject* map = game.InstantiateGameObjectBeforeStart("map", glm::vec3(0));
        GameObject* tank = game.InstantiateGameObjectBeforeStart("tank", glm::vec3(2,0,0));
        GameObject* tankhead = game.InstantiateGameObjectBeforeStart("tankhead", glm::vec3(0,0.2f,0));
        GameObject* muzzle = game.InstantiateGameObjectBeforeStart("tankhead", glm::vec3(0,0,0));
        GameObject* tankheadVisual = game.InstantiateGameObjectBeforeStart("tankhead_vis", glm::vec3(0,0,0));
        tankhead->setParent(tank);
        tankheadVisual->setParent(tankhead);
        muzzle->setParent(tankhead);
        tankheadVisual->scale = glm::vec3(0.5f);
        Camera& cam = game.frameData.camera;
        cam.position = glm::vec3(0,5,5);
        cam.target = glm::vec3(0);
        cam.up = glm::vec3(0,1,0);

        //TEST

        // map->setRotation(glm::vec3(-90.0f, 0.0f, -90.0f));
        game.createRenderInfo(*map, 1, 4);
        game.createRenderInfo(*tank, 2, 3);
        game.createRenderInfo(*tankheadVisual, 3, 3);
        tank->addComponent(new MovementBehaviour(), &game);
        tankhead->addComponent(new RotateBehaviour(), &game);
        muzzle->addComponent(new ShootBehaviour(), &game);
        muzzle->addComponent(new CamFollowBehaviour(), &game);
        muzzle->addComponent(new DivingBehaviour(), &game);


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



