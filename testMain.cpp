//
// Created by benja on 22/06/2023.
//

#include "28_model_loading.h"
#include "GameObject.h"
#include "helper.h"
#include "deltaTime.h"



struct Game {
    GameObject rootNode;
    Renderer* renderer;

    Game(Renderer* renderer) : renderer{renderer} {}

    GameObject* InstantiateGameObjectBeforeStart(const glm::vec3 &pos) {
        auto test = new GameObject(pos);
        rootNode.addChild(test);
        return test;
    }

    GameObject* InstantiateGameObjectBeforeStart(std::string& id, const glm::vec3 &pos) {
        auto test = new GameObject(id, pos);
        rootNode.addChild(test);
        return test;
    }
    GameObject* InstantiateGameObjectBeforeStart(std::string id, const glm::vec3 &pos) {
        auto test = new GameObject(std::move(id), pos);
        rootNode.addChild(test);
        return test;
    }

    GameObject* InstantiateGameObject(std::string& id, const glm::vec3 &pos) {
        auto test = new GameObject(id, pos);
        rootNode.addChild(test);
        test->start();
        return test;
    }
    GameObject* InstantiateGameObject(std::string id, const glm::vec3 &pos) {
        auto test = new GameObject(std::move(id), pos);
        rootNode.addChild(test);
        test->start();
        return test;
    }



    void mainLoop() {
        bool firstFrame{true};
        while (!glfwWindowShouldClose(renderer->window)) {
            glfwPollEvents();
            if (firstFrame && !rootNode.getChildren().empty()) {
                for(auto& child : rootNode.getChildren()){
                    std::function<void(GameObject*)> start_lambda = [](GameObject* a) {a->start();};
                    GameObject::do_for_all_nodes(child, start_lambda);
                }
            }
            firstFrame = false;
            for(auto& child : rootNode.getChildren()){
                std::function<void(GameObject*)> update_lambda = [](GameObject* a) {a->update();};
                GameObject::do_for_all_nodes(child, update_lambda);
            }
            resetFrameTime();
            renderer->drawFrame(rootNode);
        }
        vkDeviceWaitIdle(renderer->device);
    }

    void cleanupGameObjectRenderMemory(){
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

    void createRenderInfo(GameObject& obj, uint32_t textureIndex, uint32_t modelIndex) {
        if(obj.renderInfo) {
            throw std::runtime_error("creating RenderInfo on a gameObject that already has one");
        }
        obj.renderInfo = new RenderInfo;
        renderer->createRenderInfo(obj.renderInfo, textureIndex, modelIndex);
    }

};

struct test{
    std::string a;
};

int main() {
    Renderer renderer;
    Game game{&renderer};


    try {
        //setup
        renderer.model_paths = {"models/viking_room.obj", "models/sphere.obj", "models/cube.obj", "models/tank.obj", "models/map.obj"};
        renderer.texture_paths = {"textures/viking_room.png", "textures/test.png"};
        renderer.init();

        //obj setup
        GameObject* map = game.InstantiateGameObjectBeforeStart("map", glm::vec3(0));
        GameObject* tank = game.InstantiateGameObjectBeforeStart("tank", glm::vec3(2,1,0));
        tank->setParent(map);

        //TEST

        // map->setRotation(glm::vec3(-90.0f, 0.0f, -90.0f));
        game.createRenderInfo(*map, 0, 4);
        game.createRenderInfo(*tank, 1, 3);
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



