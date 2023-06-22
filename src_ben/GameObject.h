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

struct GameObject{
private:
    glm::mat4 rotation;
public:
    glm::vec3 position;
    glm::vec3 scale;
    uint32_t textureIndex;
    uint32_t modelIndex;
    std::vector<struct VkDescriptorSet_T *> descriptorSets;
    std::vector<BehaviourComponent> components;
    std::vector<struct VkDescriptorSet_T *> descriptorSets_benji;
    std::vector<struct VkBuffer_T *> uniformBuffers;
    std::vector<struct VkDeviceMemory_T *> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    struct VkDescriptorPool_T *descriptorPool;

    GameObject(uint32_t textureIndex, uint32_t modelIndex);

    GameObject(uint32_t textureIndex, uint32_t modelIndex, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);

    void start();
    void update();
    void reset();
    void setRotation(glm::vec3 rot);
    void move(glm::vec3 dir);
    void rotate(float deg, glm::vec3 axis);
    glm::mat4 getModelMatrix();

};


#endif //INC_28_MODEL_LOADING_CPP_GAMEOBJECT_H
