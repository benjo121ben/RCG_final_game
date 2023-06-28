//
// Created by benja on 22/06/2023.
//

#include <iostream>
#include "BehaviourComponent.h"
#include "deltaTime.h"
#include "Game.h"
#include "helper.h"

BehaviourComponent::~BehaviourComponent() = default;
void BehaviourComponent::start(FrameData& frameData){}
void BehaviourComponent::update(FrameData& frameData){}
void BehaviourComponent::reset(FrameData& frameData){}
void BehaviourComponent::onHit(FrameData& frameData){}

bool BehaviourComponent::key_pressed(int key){
    return Game::keymap.find(key) != Game::keymap.end() && Game::keymap.find(key)->second == 1;
}

void RotationMovementBehaviour::update(FrameData& frameData) {
    if(key_pressed(GLFW_KEY_RIGHT)) gameObject->rotate(-90, glm::vec3(0,1,0));
    if(key_pressed(GLFW_KEY_LEFT)) gameObject->rotate(90, glm::vec3(0,1,0));
}

void RotationBehaviour::update(FrameData& frameData) {
    gameObject->rotate(180, glm::vec3(0,1,1));
}



void MovementBehaviour::update(FrameData& frameData) {
    if(key_pressed(GLFW_KEY_W)) gameObject->move(glm::normalize(gameObject->to_world(glm::vec3(0,0,1), 0)) * -speed * deltaTime());
    if(key_pressed(GLFW_KEY_S)) gameObject->move(glm::normalize(gameObject->to_world(glm::vec3(0,0,1), 0)) * speed * deltaTime());
    if(key == -1 && key_pressed(GLFW_KEY_LEFT_CONTROL)) {
        speed--;
        println("speed", speed);
        key = 0;
    }
    if(key == -1 && key_pressed(GLFW_KEY_LEFT_SHIFT)) {
        speed++;
        println("speed", speed);
        key = 1;
    }
    if((!key_pressed(GLFW_KEY_LEFT_CONTROL) && key == 0) || (!key_pressed(GLFW_KEY_LEFT_SHIFT) && key == 1)){
        key = -1;
    }
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

void BackAndForthBehaviour::update(FrameData &frameData) {
    gameObject->move(glm::vec3(state, 0,0) * deltaTime());
    current += state * deltaTime();
    if(current >= 1) {
        float difference = 1 - current;
        current = 1;
        state = -1;
        gameObject->move(glm::vec3(difference, 0,0));
    }
    if(current <= -1){
        float difference = -1 - current;
        current = -1;
        state = 1;
        gameObject->move(glm::vec3(difference, 0,0));
    }
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
        GameObject* bulletParent = game->InstantiateGameObject("bulletparent", glm::vec3(gameObject->getWorldPos()));
        GameObject* bulletVisual = game->InstantiateGameObject("bulletvisual", glm::vec3(0));
        bulletVisual->setParent(bulletParent);
        bulletVisual->scale = glm::vec3(0.5f);
        bulletParent->rotation = gameObject->get_world_rotation_matrix();
        game->createRenderInfo(*bulletVisual, 0, 1);
        bulletParent->addComponent(new BulletBehaviour(current), game);
        bulletVisual->addComponent(new RotationBehaviour(), game);
        CircleBound* hitbox = bulletParent->addCirclebound(game);
        hitbox->radius = 0.25f;
        bulletParent->start(frameData);
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
        return;
    }

}

void DisappearOnHitBehaviour::onHit(FrameData &frameData) {
    game->scheduleGameObjectRemoval(gameObject);
}

