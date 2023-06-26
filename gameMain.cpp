//
// Created by benja on 22/06/2023.
//

#include "28_model_loading.h"
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
    test->start(Game::keymap);
    return test;
}
GameObject* Game::InstantiateGameObject(std::string id, const glm::vec3 &pos) {
    auto test = new GameObject(std::move(id), pos);
    rootNode.addChild(test);
    test->start(Game::keymap);
    return test;
}



void Game::mainLoop() {
    bool firstFrame{true};
    while (!glfwWindowShouldClose(renderer->window)) {
        glfwPollEvents();
        if (firstFrame && !rootNode.getChildren().empty()) {
            for(auto& child : rootNode.getChildren()){
                std::function<void(GameObject*)> start_lambda = [&](GameObject* a) {a->start(Game::keymap);};
                GameObject::do_for_all_nodes(child, start_lambda);
            }
        }
        firstFrame = false;
        for(auto& child : rootNode.getChildren()){
            std::function<void(GameObject*)> update_lambda = [&](GameObject* a) {a->update(Game::keymap);};
            GameObject::do_for_all_nodes(child, update_lambda);
        }
        resetFrameTime();
        renderer->drawFrame(rootNode);
    }
    vkDeviceWaitIdle(renderer->device);
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
        GameObject* tank = game.InstantiateGameObjectBeforeStart("tank", glm::vec3(2,1,0));
        tank->setParent(map);

        //TEST

        // map->setRotation(glm::vec3(-90.0f, 0.0f, -90.0f));
        game.createRenderInfo(*map, 1, 4);
        game.createRenderInfo(*tank, 2, 3);
        tank->addComponent(new RotateBehaviour());


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



