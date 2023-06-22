//
// Created by benja on 23/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_RENDERINFO_H
#define INC_28_MODEL_LOADING_CPP_RENDERINFO_H
struct RenderInfo{
    uint32_t textureIndex;
    uint32_t modelIndex;
    bool visible = true;
    std::vector<struct VkDescriptorSet_T *> descriptorSets;
    std::vector<struct VkDescriptorSet_T *> descriptorSets_benji;
    std::vector<struct VkBuffer_T *> uniformBuffers;
    std::vector<struct VkDeviceMemory_T *> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    struct VkDescriptorPool_T *descriptorPool;
};
#endif //INC_28_MODEL_LOADING_CPP_RENDERINFO_H
