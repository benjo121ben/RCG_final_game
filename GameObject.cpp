//
// Created by benja on 22/06/2023.
//

#include "GameObject.h"
#include "glm/gtx/euler_angles.hpp"

GameObject::GameObject() = default;
GameObject::GameObject(glm::vec3 position) : position{position} {}

GameObject::GameObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : GameObject(position){
    this->scale = scale;
    this->setRotation(rotation);
}


void GameObject::start() {for (auto& component : components) component.start();}

void GameObject::update() {for ( auto& component : components) component.update();}

void GameObject::reset() {for (auto& component : components) component.reset();}

void GameObject::setRotation(glm::vec3 rot) {this->rotation = glm::eulerAngleXYZ(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));}

void GameObject::move(glm::vec3 dir) {position += dir;}

void GameObject::rotate(float deg, glm::vec3 axis) {rotation = glm::rotate(rotation, glm::radians(deg), axis);}

void GameObject::setVisible(bool visible){
    if(!renderInfo) return;
    renderInfo->visible = visible;
}

bool GameObject::is_static() const{
    return static_state;
}

glm::mat4 GameObject::getModelMatrix() const {
    if(this->is_static()){
        return model_cache;
    }
    return glm::scale(
            rotation * glm::translate(
                    glm::mat4(1.0f),
                    position
            ),
            scale
    );
}

void GameObject::set_static(bool state){
    if(state){
        model_cache = getModelMatrix();
    }
    this->static_state = state;
}


