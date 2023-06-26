//
// Created by benja on 22/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
#define INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
#include<map>

struct GameObject;

struct BehaviourComponent {
    GameObject* gameObject = nullptr;
    virtual void start(const std::map<int, int>& keymap);
    virtual void update(const std::map<int, int>& keymap);
    virtual void reset(const std::map<int, int>& keymap);
    static bool key_pressed(const std::map<int, int>& keymap, int key);
};

class RotateBehaviour : public BehaviourComponent {
    void update(const std::map<int, int>& keymap) override;
};

#include "GameObject.h"

#endif //INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
