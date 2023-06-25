//
// Created by benja on 22/06/2023.
//

#include "BehaviourComponent.h"

void BehaviourComponent::start(){}
void BehaviourComponent::update(){}
void BehaviourComponent::reset(){}

void RotateBehaviour::update() {this->gameObject->rotate(90, glm::vec3(0,1,0));}