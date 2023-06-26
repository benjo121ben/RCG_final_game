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
void println(std::string str,T any) {
    std::cout << str << " " << any << "\n";
}

template <typename T>
void println(T str) {
    std::cout << str << "\n";
}

inline void println(glm::mat4 mat) {
    std::cout << glm::to_string(mat) << "\n";
}

inline void println(std::string str, glm::mat4 mat) {
    std::cout << str << " " << glm::to_string(mat) << "\n";
}

template <typename T>
void print(std::string str, T any) {
    std::cout << str << " " << any;
}

template <typename T>
void print(T str) {
    std::cout << str;
}

#endif //INC_28_MODEL_LOADING_CPP_HELPER_H
