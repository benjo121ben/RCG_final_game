//
// Created by benja on 22/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_GAMEOBJECT_H
#define INC_28_MODEL_LOADING_CPP_GAMEOBJECT_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include "framedata.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include "glm/gtx/euler_angles.hpp"
#include <map>
#include "RenderInfo.h"

struct Game;
struct BehaviourComponent;
struct CircleBound;
struct CubeBound;

struct GameObject{
    CircleBound* test = nullptr;
    CubeBound* test2 = nullptr;
    std::string id = "node";
    glm::vec3 position = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);
    glm::mat4 rotation = glm::eulerAngleXYZ(0.0f,0.0f,0.0f);
    RenderInfo* renderInfo = nullptr;
    GameObject* parent = nullptr;

    bool isRoot = false;

private:
    BehaviourComponent* components [5];
    int componentsize = 0;
    std::vector<GameObject*> children;
    bool static_state = false;
    glm::mat4 model_cache = glm::mat4(1.0f);


public:
    static void do_for_all_nodes(GameObject* node, std::function< void(GameObject*) >& lambda);

    GameObject();
    GameObject(std::string id, glm::vec3 position);
    explicit GameObject(glm::vec3 position);

    GameObject(std::string id, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
    ~GameObject();

    void setVisible(bool visible);
    void start(FrameData& frameData);
    void update(FrameData& frameData);
    void reset(FrameData& frameData);
    void onHit(FrameData& frameData);
    void setRotation(glm::vec3 rot);

    void addComponent(BehaviourComponent* component, Game* game);
    CircleBound* addCirclebound(Game* game);
    CircleBound* getCirclebound();
    CubeBound* addCubebound(Game* game);
    CubeBound* getCubebound();
    void addChild(GameObject* child, bool setParent=true);
    void removeChild(GameObject* child);
    void setParent(GameObject* parent, bool addChild=true);
    void move(glm::vec3 dir);
    void rotate(float deg, glm::vec3 axis);
    void set_static(bool state);
    [[nodiscard]] glm::mat4 get_world_rotation_matrix() const;
    [[nodiscard]] glm::vec3 to_world(glm::vec3 vector, int point_or_vec) const;
    [[nodiscard]] glm::vec3 getWorldPos() const;
    [[nodiscard]] GameObject* getParent() const;
    [[nodiscard]] const std::vector<GameObject*>& getChildren() const;
    [[nodiscard]] glm::mat4 getModelMatrix() const;
    [[nodiscard]] bool is_static() const;


};


#endif //INC_28_MODEL_LOADING_CPP_GAMEOBJECT_H
