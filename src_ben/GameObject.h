//
// Created by benja on 22/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_GAMEOBJECT_H
#define INC_28_MODEL_LOADING_CPP_GAMEOBJECT_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include "BehaviourComponent.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include "glm/gtx/euler_angles.hpp"
#include "RenderInfo.h"

struct GameObject{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);
    RenderInfo* renderInfo = nullptr;

private:
    glm::mat4 rotation = glm::eulerAngleXYZ(0.0f,0.0f,0.0f);
    bool static_state = false;
    glm::mat4 model_cache = glm::mat4(1.0f);
    std::vector<BehaviourComponent> components;


public:
    GameObject();
    explicit GameObject(glm::vec3 position);

    GameObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);

    void setVisible(bool visible);
    [[nodiscard]] bool is_static() const;

    void start();
    void update();
    void reset();
    void setRotation(glm::vec3 rot);
    void move(glm::vec3 dir);
    void rotate(float deg, glm::vec3 axis);
    [[nodiscard]] glm::mat4 getModelMatrix() const;
    void set_static(bool state);


};


#endif //INC_28_MODEL_LOADING_CPP_GAMEOBJECT_H
