//
// Created by benja on 27/06/2023.
//
#include "bounds.h"
#include "GameObject.h"
std::vector<glm::vec3> CubeBound::getWorldPoints(){
    std::vector<glm::vec3> ret;
    for(const auto& p : points){
        ret.emplace_back(glm::vec3 (gameObject->getModelMatrix() * glm::vec4 (p,1)));
    }
    return ret;
}

CubeBound::CubeBound(GameObject* obj) : gameObject{obj}{}

CircleBound::CircleBound(GameObject* obj) : gameObject{obj}{}

glm::vec3 CircleBound::getWorldPoint() const{
    return gameObject->getModelMatrix() * glm::vec4 (point,1);
}

bool CircleBound::circle_circle(CircleBound* bound,CircleBound* bound2){
    return (bound->radius + bound2->radius) >= glm::distance(bound->getWorldPoint(),bound2->getWorldPoint());
}