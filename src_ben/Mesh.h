//
// Created by benja on 23/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_MESH_H
#define INC_28_MODEL_LOADING_CPP_MESH_H

#include "Vertex.h"

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    struct VkBuffer_T *vertexBuffer;
    struct VkDeviceMemory_T *vertexBufferMemory;
    struct VkBuffer_T *indexBuffer;
    struct VkDeviceMemory_T *indexBufferMemory;
};


#endif //INC_28_MODEL_LOADING_CPP_MESH_H
