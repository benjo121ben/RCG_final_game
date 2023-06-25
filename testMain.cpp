//
// Created by benja on 22/06/2023.
//

#include "28_model_loading.h"
#include "GameObject.h"
#include "helper.h"
#include "deltaTime.h"



struct Game {
    std::vector<GameObject> gameObjectList;
    Renderer* renderer;

    Game(Renderer* renderer) : renderer{renderer} {}

    GameObject &InstantiateGameObjectBeforeStart(const glm::vec3 &pos) {
        GameObject &obj = gameObjectList.emplace_back(
                GameObject(pos, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
        return obj;
    }

    GameObject &InstantiateGameObject(const glm::vec3 &pos) {
        GameObject &obj = gameObjectList.emplace_back(
                GameObject(pos, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
        obj.start();
        return obj;
    }



    void mainLoop() {
        bool firstFrame{true};
        bool first{true};
        while (!glfwWindowShouldClose(renderer->window)) {
            glfwPollEvents();
            if (firstFrame) {
                for (auto &gameObj : gameObjectList) {
                    gameObj.start();
                }
                firstFrame = false;
            }
            if (fullTime() > 2.0f && first) {
                println("TRIGGERED");
                GameObject& obj = InstantiateGameObject(glm::vec3(0, 1, 0));
                GameObject& obj2 = InstantiateGameObject(glm::vec3(0, 1, 0));
                obj.scale = glm::vec3(0.1f, 0.1f, 0.1f);
                obj2.scale = glm::vec3(0.1f, 0.1f, 0.1f);
                obj.setParent(&gameObjectList[0]);
                createRenderInfo(obj, 1, 1);
                createRenderInfo(obj2, 1, 1);
                first = false;
            }
            gameObjectList[0].rotate(10, glm::vec3(0,1,0));
            resetFrameTime();
            renderer->drawFrame(gameObjectList);
        }
        vkDeviceWaitIdle(renderer->device);
    }

    void cleanupGameObjects(){
        for(auto& obj : gameObjectList) {
            if(obj.renderInfo){
                renderer->cleanupRenderInfo(obj.renderInfo);
                delete obj.renderInfo;
                obj.renderInfo = nullptr;
            }
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

int main() {
    Renderer renderer;
    Game game{&renderer};

    GameObject & obj = game.InstantiateGameObjectBeforeStart(glm::vec3(0));
    obj.setRotation(glm::vec3(-90.0f, 0.0f, -90.0f));
    try {
        renderer.model_paths = {"models/viking_room.obj", "models/sphere.obj", "models/cube.obj"};
        renderer.texture_paths = {"textures/viking_room.png", "textures/test.png"};
        renderer.init();
        game.createRenderInfo(obj, 0, 0);
        resetGameTime();
        resetFrameTime();
        game.mainLoop();
        game.cleanupGameObjects();
        renderer.cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}



