//
// Created by benja on 22/06/2023.
//

#include <stdexcept>
#include "helper.h"
#include "GameObject.h"
#include "glm/gtx/euler_angles.hpp"

GameObject::GameObject() = default;
GameObject::GameObject(glm::vec3 position) : position{position} {}

GameObject::GameObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : GameObject(position){
    this->scale = scale;
    this->setRotation(rotation);
}

void GameObject::addChild(GameObject* child, bool setParent) {
    if(child == this) throw std::runtime_error("trying to add object as its own child");
    println(children.size());
    for(auto it{children.begin()}; it != children.end(); ++it){
        println("ey");
        if(*it == child){
            println("already child");
            return;
        }
    }
    children.push_back(child);
    if(setParent){
        child->setParent(this, false);
    }
    println("child added", child);
}

void GameObject::removeChild(GameObject* remChild) {
    int i = 0;
    for(auto it{children.begin()}; it != children.end(); ++it){
        if(*it == remChild){
            println("ERASED", *it);
            if ((*it)->parent == this){
                (*it)->parent = nullptr;
            }
            children.erase(it);
            return;
        }
        i++;
    }
    println("NONE ERASED");
}

void GameObject::setParent(GameObject* newparent, bool addChild) {
    if(parent == newparent){
        println("PARENT ALREADY PARENT");
        return;
    }
    if(parent){
        parent->removeChild(this);
    }
    parent = newparent;
    if(addChild && parent){
        newparent->addChild(this, false);
    }
}

GameObject* GameObject::getParent() const{
    return parent;
}

const std::vector<GameObject*>& GameObject::getChildren() const{
    return children;
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
    if(!parent) {
        return glm::scale(
                rotation * glm::translate(
                        glm::mat4(1.0f),
                        position
                ),
                scale
        );
    }
    else {
        return glm::scale(
                rotation * glm::translate(
                        glm::mat4(1.0f),
                        position
                ),
                scale
        ) * parent->getModelMatrix();
    }
}

void GameObject::set_static(bool state){
    if(state){
        model_cache = getModelMatrix();
    }
    this->static_state = state;
}


