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
        bool first{true};
        while (!glfwWindowShouldClose(renderer->window)) {
            glfwPollEvents();
            if (firstFrame && !rootNode.getChildren().empty()) {
                for(auto& child : rootNode.getChildren()){
                    std::function<void(GameObject*)> start_lambda = [](GameObject* a) {a->start();};
                    GameObject::do_for_all_nodes(child, start_lambda);
                }
            }
            firstFrame = false;

            if (fullTime() > 2.0f && first) {
                println("TRIGGERED");
                GameObject* obj = InstantiateGameObject("circle1", glm::vec3(0, 1, 0));
                GameObject* obj2 = InstantiateGameObject("circle2", glm::vec3(0, 1, 0));
                obj->scale = glm::vec3(0.1f, 0.1f, 0.1f);
                obj2->scale = glm::vec3(0.1f, 0.1f, 0.1f);
                obj->setParent(obj2);
                createRenderInfo(*obj, 1, 1);
                createRenderInfo(*obj2, 1, 1);
                first = false;
            }
            // println("mat");
            // for(const auto& o : gameObjectList){
            //     println(o.id);
            //     if(o.getParent()){
            //         print("PAR ADR: ");
            //         println(o.getParent()->id);
            //         println("PAR MAT: ", o.getParent()->getModelMatrix());
            //         println("PAR COMBO: ",o.getParent()->getModelMatrix() * glm::scale(
            //                 o.rotation * glm::translate(
            //                         glm::mat4(1.0f),
            //                         o.position
            //                 ),
            //                 o.scale
            //         ));
            //         println("");
            //     }
            //     println(o.getModelMatrix());
            //     println("");
            // }
            // println("mat end");
            if(!first) rootNode.getChildren()[0]->rotate(90, glm::vec3(0,1,0));
            //println(rootNode.getChildren()[0]->id);
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
        renderer.model_paths = {"models/viking_room.obj", "models/sphere.obj", "models/cube.obj"};
        renderer.texture_paths = {"textures/viking_room.png", "textures/test.png"};
        renderer.init();

        //obj setup
        GameObject* emptyobj = game.InstantiateGameObjectBeforeStart(glm::vec3(2, 0, 0));
        println("");
        println("objp", &emptyobj);
        println("obj0", &(game.rootNode.getChildren()[0]));
        println("");

        GameObject* viking = game.InstantiateGameObjectBeforeStart("vikingRoom", glm::vec3(0));
        viking->setParent(emptyobj);
        println("adresses");
        println("objp", &emptyobj);
        println("viki", &viking);
        println("obj0", &(game.rootNode.getChildren()[0]));
        println("obj1", &(game.rootNode.getChildren()[1]));
        println("");

        println("ids");
        println("objp", emptyobj->id);
        println("viki", viking->id);
        println("obj0", game.rootNode.getChildren()[0]->id);
        println("obj1", game.rootNode.getChildren()[1]->id);
        println("");

        println("parent?");
        println("objp", emptyobj->getParent() != nullptr);
        println("viki", viking->getParent() != nullptr);
        println("obj0", game.rootNode.getChildren()[0]->getParent() != nullptr);
        println("obj1", game.rootNode.getChildren()[1]->getParent() != nullptr);
        println("");

        emptyobj->id = "not rooot";
        println("ids after change");
        println("objp", emptyobj->id);
        println("viki", viking->id);
        println("obj0", game.rootNode.getChildren()[0]->id);
        println("obj1", game.rootNode.getChildren()[1]->id);
        println("");


        //TEST

        viking->setRotation(glm::vec3(-90.0f, 0.0f, -90.0f));
        game.createRenderInfo(*viking, 0, 0);

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



