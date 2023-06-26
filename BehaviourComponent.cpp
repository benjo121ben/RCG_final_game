//
// Created by benja on 22/06/2023.
//

#include <iostream>
#include "BehaviourComponent.h"
#include "deltaTime.h"
#include "Game.h"
#include "glm/gtx/string_cast.hpp"

BehaviourComponent::~BehaviourComponent() = default;
void BehaviourComponent::start(FrameData& frameData){}
void BehaviourComponent::update(FrameData& frameData){}
void BehaviourComponent::reset(FrameData& frameData){}

bool BehaviourComponent::key_pressed(int key){
    return Game::keymap.find(key) != Game::keymap.end() && Game::keymap.find(key)->second == 1;
}

void RotateBehaviour::update(FrameData& frameData) {
    if(key_pressed(GLFW_KEY_RIGHT)) gameObject->rotate(-90, glm::vec3(0,1,0));
    if(key_pressed(GLFW_KEY_LEFT)) gameObject->rotate(90, glm::vec3(0,1,0));
}



void MovementBehaviour::update(FrameData& frameData) {
    if(key_pressed(GLFW_KEY_W)) gameObject->move(glm::normalize(gameObject->to_world(glm::vec3(0,0,-1), 0)) * deltaTime());
    if(key_pressed(GLFW_KEY_S)) gameObject->move(glm::normalize(gameObject->to_world(glm::vec3(0,0,1), 0)) * deltaTime());
    if(key_pressed(GLFW_KEY_D)) {
        gameObject->rotate(-90, glm::vec3(0, 1, 0));
    }
    if(key_pressed(GLFW_KEY_A)) {
        gameObject->rotate(90, glm::vec3(0, 1, 0));
    }
}

void CamFollowBehaviour::update(FrameData& frameData) {
    frameData.camera.position = gameObject->to_world(glm::vec3(0),1) + gameObject->to_world(glm::vec3(0,0.45f,1), 0);
    frameData.camera.target = gameObject->to_world(glm::vec3(0,0.28f,0), 1);
}

void ShootBehaviour::update(FrameData& frameData) {
    if(key_pressed(GLFW_KEY_UP) && current < upperLimit){
        frameData.camera.offset_tar += glm::vec3 (0,0.3f,0) * deltaTime();
        current += deltaTime();
    }
    if(key_pressed(GLFW_KEY_DOWN) && current > lowerLimit){
        game->frameData.camera.offset_tar -= glm::vec3 (0,0.3f,0) * deltaTime();
        current -= deltaTime();
    }
    if(key_pressed(GLFW_KEY_SPACE) && ready) {
        ready = false;
        GameObject* g = game->InstantiateGameObject("TEST", glm::vec3(gameObject->getWorldPos()));
        g->rotation = gameObject->get_world_rotation_matrix();
        game->createRenderInfo(*g,0,2);
        g->addComponent(new BulletBehaviour(current), game);
        g->start(frameData);
    }
    if(!key_pressed(GLFW_KEY_SPACE) && !ready) {
        ready = true;
    }

}

BulletBehaviour::BulletBehaviour(float angle) : angle{angle}{}

void BulletBehaviour::start(FrameData &frameData) {
    currentspeed = gameObject->to_world(glm::vec3(0,angle * 15,-20), 0);
}

void BulletBehaviour::update(FrameData& frameData) {
    currentspeed += gameObject->to_world(glm::vec3(0,-9.81,0.01f),0) * deltaTime();
    gameObject->move(currentspeed * deltaTime());
    timer += deltaTime();
    if(time < timer){
        game->scheduleGameObjectRemoval(gameObject);
        std::cout << "destroy bullet\n";
        return;
    }

}

