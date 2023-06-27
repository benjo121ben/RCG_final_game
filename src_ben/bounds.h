//
// Created by benja on 27/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_BOUNDS_H
#define INC_28_MODEL_LOADING_CPP_BOUNDS_H
#include"glm/glm.hpp"

#include<vector>
struct GameObject;

struct CubeBound{
    bool is_static = false;
    glm::vec3 points[8] = {
            glm::vec3(0.5f,0.5f,0.5f),
            glm::vec3(0.5f,0.5f,-0.5f),
            glm::vec3(-0.5f,0.5f,-0.5f),
            glm::vec3(-0.5f,0.5f,0.5f),
            glm::vec3(0.5f,-0.5f,0.5f),
            glm::vec3(0.5f,-0.5f,-0.5f),
            glm::vec3(-0.5f,-0.5f,-0.5f),
            glm::vec3(-0.5f,-0.5f,0.5f),
    };
    GameObject* gameObject = nullptr;
    std::vector<glm::vec3> getWorldPoints();
    explicit CubeBound(GameObject* obj);
};

struct CircleBound{
public:
    glm::vec3 point = glm::vec3(0);
    float radius = 0.5f;
    GameObject* gameObject = nullptr;
    [[nodiscard]] glm::vec3 getWorldPoint() const;

    explicit CircleBound(GameObject* obj);
    static bool circle_circle(CircleBound* bound,CircleBound* bound2);
};

#endif //INC_28_MODEL_LOADING_CPP_BOUNDS_H
