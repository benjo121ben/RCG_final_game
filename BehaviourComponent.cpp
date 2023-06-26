//
// Created by benja on 22/06/2023.
//

#include "BehaviourComponent.h"
#include "deltaTime.h"

void BehaviourComponent::start(const std::map<int, int>& keymap){}
void BehaviourComponent::update(const std::map<int, int>& keymap){}
void BehaviourComponent::reset(const std::map<int, int>& keymap){}

bool BehaviourComponent::key_pressed(const std::map<int, int>& keymap, int key){
    return keymap.find(key) != keymap.end() && keymap.find(key)->second == 1;
}

void RotateBehaviour::update(const std::map<int, int>& keymap) {
    if(key_pressed(keymap, GLFW_KEY_W)) gameObject->move(glm::vec3(1,0,0) * deltaTime());
    if(key_pressed(keymap, GLFW_KEY_R)) gameObject->rotate(90, glm::vec3(0,1,0));
    if(key_pressed(keymap, GLFW_KEY_S)) gameObject->scale += glm::vec3(1) * deltaTime();
}