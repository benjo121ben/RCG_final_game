//
// Created by benja on 25/06/2023.
//

#include "deltaTime.h"
float deltaTime() {
    return std::chrono::duration<float, std::chrono::seconds::period>(
            std::chrono::high_resolution_clock::now() - lastFrameTime).count();
}

float fullTime() {
    return std::chrono::duration<float, std::chrono::seconds::period>(
            std::chrono::high_resolution_clock::now() - startGameTime).count();
}

void resetGameTime() {
    startGameTime = std::chrono::high_resolution_clock::now();
}

void resetFrameTime() {
    lastFrameTime = std::chrono::high_resolution_clock::now();
}