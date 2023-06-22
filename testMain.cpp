//
// Created by benja on 22/06/2023.
//

#include "28_model_loading.h"
#include "GameObject.h"
#include "BehaviourComponent.h"


void register_texture_and_models(){

}








int main() {
    HelloTriangleApplication app;
    register_texture_and_models();

    GameObject & obj = app.InstantiateGameObjectBeforeStart(0,0,glm::vec3(0));
    obj.setRotation(glm::vec3(-90.0f, 0.0f, -90.0f));
    try {
        app.model_paths = {"models/viking_room.obj", "models/sphere.obj", "models/cube.obj"};
        app.texture_paths = {"textures/viking_room.png", "textures/test.png"};
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

