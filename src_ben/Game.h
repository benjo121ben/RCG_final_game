//
// Created by benja on 26/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_GAME_H
#define INC_28_MODEL_LOADING_CPP_GAME_H
#include "framedata.h"
#include "bounds.h"
#include <queue>

struct Renderer;
struct GameObject;
struct GLFWwindow;


struct Game {
private:
    std::queue<GameObject*> removalQueue;
public:
    GameObject *rootNode;
    GameObject *player;
    FrameData frameData;
    inline static std::map<int, int> keymap = std::map<int,int>();
    Renderer *renderer;
    std::vector<CircleBound *> circleBounds;
    std::vector<CubeBound *> cubeBounds;

    explicit Game(Renderer *renderer);
    ~Game();

    GameObject *InstantiateGameObjectBeforeStart(const glm::vec3 &pos);

    GameObject *InstantiateGameObjectBeforeStart(std::string &id, const glm::vec3 &pos);

    GameObject *InstantiateGameObjectBeforeStart(std::string id, const glm::vec3 &pos);

    GameObject *InstantiateGameObject(std::string &id, const glm::vec3 &pos);

    GameObject *InstantiateGameObject(std::string id, const glm::vec3 &pos);

    void scheduleGameObjectRemoval(GameObject *);

    void destroyScheduledGameObjects();

    void mainLoop();

    void check_hitboxes();

    void cleanupAllGameObjectRenderMemory() const;
    void cleanupSingleGameObjectRenderInfo(GameObject * obj) const;

    void createRenderInfo(GameObject& obj, uint32_t textureIndex, uint32_t modelIndex) const;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
#endif //INC_28_MODEL_LOADING_CPP_GAME_H
