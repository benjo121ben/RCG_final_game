//
// Created by benja on 22/06/2023.
//

#include "BehaviourComponent.h"
#include "deltaTime.h"

void BehaviourComponent::start(){}
void BehaviourComponent::update(){}
void BehaviourComponent::reset(){}

void RotateBehaviour::update() {
    if(fullTime() < 1) gameObject->position = glm::vec3(0,0,0);
    if(fullTime() < 4) gameObject->move(glm::vec3(1,0,0) * deltaTime());
    gameObject->rotate(90, glm::vec3(0,1,0));
    if(fullTime() < 4) gameObject->scale += glm::vec3(1) * deltaTime();
}