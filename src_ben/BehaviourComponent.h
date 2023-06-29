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
    bool enabled = true;
    GameObject* gameObject = nullptr;
    Game* game = nullptr;
    virtual ~BehaviourComponent();
    virtual void start(FrameData& frameData);
    virtual void update(FrameData& frameData);
    virtual void reset(FrameData& frameData);
    virtual void onHit(FrameData& frameData, bool otherIsStatic);
    static bool key_pressed(int key);
};

struct RotationMovementBehaviour : public BehaviourComponent {
    void update(FrameData& frameData) override;
};

struct RotationBehaviour : public BehaviourComponent {
    void update(FrameData& frameData) override;
};


struct MovementBehaviour : public BehaviourComponent {
    int key = -1;
    float speed = 3.0f;
    void update(FrameData& frameData) override;
};

struct DisappearOnHitBehaviour : public BehaviourComponent {
    void onHit(FrameData& frameData, bool otherIsStatic) override;
};

struct CamFollowBehaviour : public BehaviourComponent {
    void update(FrameData& frameData) override;
};

struct BackAndForthBehaviour : public BehaviourComponent {
    glm::vec3 startPos;
    glm::vec3 endPos;
    float time;
    float current = 0;
    int state = 1;
    BackAndForthBehaviour(glm::vec3 startPos, glm::vec3 endPos, float time);
    void update(FrameData& frameData) override;
};

struct ShootBehaviour : public BehaviourComponent {
    float upperLimit = 1;
    float lowerLimit = 0;
    float current = 0;
    bool ready = true;
    void update(FrameData& frameData) override;
};

struct EnemyTargetBehaviour : public BehaviourComponent{
    static int nr;
    float time = 5;
    float timer = 0;
    explicit EnemyTargetBehaviour(float time);
    void update(FrameData& frameData) override;

};

struct BulletBehaviour : public BehaviourComponent {
    explicit BulletBehaviour(float angle);
    bool activeHitbox = false;
    float angle = 0;
    glm::vec3 currentspeed;
    float time = 5;
    float timer = 0;
    void start(FrameData& frameData) override;
    void update(FrameData& frameData) override;
};

struct EnemyBulletBehaviour: public BulletBehaviour {
    glm::vec3 dir;
    float speed = 10;
    explicit EnemyBulletBehaviour(glm::vec3 dir);
    void start(FrameData& frameData) override;
    void update(FrameData& frameData) override;
};

struct HealthBehaviour: public BehaviourComponent {
    float health = 4;
    void onHit(FrameData& frameData, bool otherIsStatic) override;
};


#include "GameObject.h"

#endif //INC_28_MODEL_LOADING_CPP_BEHAVIOURCOMPONENT_H
