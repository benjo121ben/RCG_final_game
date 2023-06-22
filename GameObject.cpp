//
// Created by benja on 22/06/2023.
//

#include "GameObject.h"
#include "glm/gtx/euler_angles.hpp"


void GameObject::start() {for (auto& component : components) component.start();}
void GameObject::update() {for ( auto& component : components) component.update();}
void GameObject::reset() {for (auto& component : components) component.reset();}
void GameObject::setRotation(glm::vec3 rot) {this->rotation = glm::eulerAngleXYZ(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));}
void GameObject::move(glm::vec3 dir) {position += dir;}
void GameObject::rotate(float deg, glm::vec3 axis) {rotation = glm::rotate(rotation, glm::radians(deg), axis);}
glm::mat4 GameObject::getModelMatrix() {
    return glm::scale(
            rotation * glm::translate(
                    glm::mat4(1.0f),
                    position
            ),
            this->scale
    );
}

GameObject::GameObject(uint32_t textureIndex, uint32_t modelIndex) : textureIndex{textureIndex}, modelIndex{modelIndex}, position{glm::vec3(0.0f)}, scale{glm::vec3(1.0f)} {
    this->rotation = glm::eulerAngleXYZ(0,0,0);
}

GameObject::GameObject(uint32_t textureIndex, uint32_t modelIndex, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : GameObject(textureIndex, modelIndex){
this->position = position;
this->scale = scale;
this->setRotation(rotation);
}