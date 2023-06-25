//
// Created by benja on 22/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
#define INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H

struct GameObject;

struct BehaviourComponent {
    GameObject* gameObject = nullptr;
    virtual void start();
    virtual void update();
    virtual void reset();
};

class RotateBehaviour : public BehaviourComponent {
    void update() override;
};

#include "GameObject.h"

#endif //INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
