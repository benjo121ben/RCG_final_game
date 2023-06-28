//
// Created by benja on 22/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
#define INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
#include <map>
#include "framedata.h"

struct GameObject;
struct Game;


struct BehaviourComponent {
    GameObject* gameObject = nullptr;
    Game* game = nullptr;
    virtual ~BehaviourComponent();
    virtual void start(FrameData& frameData);
    virtual void update(FrameData& frameData);
    virtual void reset(FrameData& frameData);
    virtual void onHit(FrameData& frameData);
    static bool key_pressed(int key);
};

class RotationMovementBehaviour : public BehaviourComponent {
    void update(FrameData& frameData) override;
};

class RotationBehaviour : public BehaviourComponent {
    void update(FrameData& frameData) override;
};


class MovementBehaviour : public BehaviourComponent {
    int key = -1;
    float speed = 10.0f;
    void update(FrameData& frameData) override;
};

class DisappearOnHitBehaviour : public BehaviourComponent {
    void onHit(FrameData& frameData) override;
};

class CamFollowBehaviour : public BehaviourComponent {
    void update(FrameData& frameData) override;
};

class BackAndForthBehaviour : public BehaviourComponent {
    float upperLimit = 1;
    float lowerLimit = 0;
    float current = 0;
    int state = 1;
    void update(FrameData& frameData) override;
};

class ShootBehaviour : public BehaviourComponent {
    float upperLimit = 1;
    float lowerLimit = 0;
    float current = 0;
    bool ready = true;
    void update(FrameData& frameData) override;
};

class BulletBehaviour : public BehaviourComponent {
public:
    BulletBehaviour(float angle);
private:
    float angle = 0;
    glm::vec3 currentspeed;
    float time = 5;
    float speed = 10;
    float timer = 0;
    void start(FrameData& frameData) override;
    void update(FrameData& frameData) override;
};


#include "GameObject.h"

#endif //INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
