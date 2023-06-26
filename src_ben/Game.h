//
// Created by benja on 26/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_GAME_H
#define INC_28_MODEL_LOADING_CPP_GAME_H
struct Game {
    GameObject rootNode;
    Renderer *renderer;
    inline static std::map<int, int> keymap = std::map<int,int>();

    explicit Game(Renderer *renderer);

    GameObject *InstantiateGameObjectBeforeStart(const glm::vec3 &pos);

    GameObject *InstantiateGameObjectBeforeStart(std::string &id, const glm::vec3 &pos);

    GameObject *InstantiateGameObjectBeforeStart(std::string id, const glm::vec3 &pos);

    GameObject *InstantiateGameObject(std::string &id, const glm::vec3 &pos);

    GameObject *InstantiateGameObject(std::string id, const glm::vec3 &pos);

    void mainLoop();

    void cleanupGameObjectRenderMemory();

    void createRenderInfo(GameObject& obj, uint32_t textureIndex, uint32_t modelIndex);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
#endif //INC_28_MODEL_LOADING_CPP_GAME_H
