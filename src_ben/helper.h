//
// Created by benja on 23/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_HELPER_H
#define INC_28_MODEL_LOADING_CPP_HELPER_H
#define GLM_ENABLE_EXPERIMENTAL
#include<iostream>
#include<string>
#include <glm/gtx/string_cast.hpp>





template <typename T>
void println(T str) {
    std::cout << str << "\n";
}

template <typename T>
void print(T str) {
    std::cout << str;
}

void println(glm::vec3 vector);

void println(glm::mat4 mat);

void println(std::string str, glm::mat4 mat);

void println(std::string str, glm::vec3 vector);



#endif //INC_28_MODEL_LOADING_CPP_HELPER_H
