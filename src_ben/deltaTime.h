//
// Created by benja on 25/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_TIME_H
#define INC_28_MODEL_LOADING_CPP_TIME_H
#include<chrono>

static auto startGameTime = std::chrono::high_resolution_clock::now();
static auto lastFrameTime = std::chrono::high_resolution_clock::now();



float deltaTime();
float fullTime();
void resetGameTime();
void resetFrameTime();
#endif //INC_28_MODEL_LOADING_CPP_TIME_H
