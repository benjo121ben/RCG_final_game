//
// Created by benja on 26/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_CAMERA_H
#define INC_28_MODEL_LOADING_CPP_CAMERA_H
#include <glm/glm.hpp>

struct Camera{
    glm::vec3 position;
    glm::vec3 offset_pos = glm::vec3(0);
    glm::vec3 up;
    glm::vec3 target;
    glm::vec3 offset_tar = glm::vec3(0);
};
#endif //INC_28_MODEL_LOADING_CPP_CAMERA_H
