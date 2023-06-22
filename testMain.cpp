//
// Created by benja on 22/06/2023.
//

#include "28_model_loading.h"


void register_texture_and_models(){
    model_paths = {"models/viking_room.obj", "models/sphere.obj", "models/cube.obj"};
    texture_paths = {"textures/viking_room.png", "textures/test.png"};
}








int main() {
    HelloTriangleApplication app;
    register_texture_and_models();
    app.gameObjectList.emplace_back(GameObject(0,0));
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

