//
// Created by benja on 22/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
#define INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H


#include "GameObject.h"

struct BehaviourComponent {
    GameObject* gameObject;
    virtual void start();
    virtual void update();
    virtual void reset();
};

class RotateBehaviour : public BehaviourComponent {
    virtual void update();
};


#endif //INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
