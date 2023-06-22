//
// Created by benja on 23/06/2023.
//

#ifndef INC_28_MODEL_LOADING_CPP_HELPER_H
#define INC_28_MODEL_LOADING_CPP_HELPER_H

template <typename T>
void println(std::string str,T any) {
    std::cout << str << " " << any << "\n";
}

template <typename T>
void println(T str) {
    std::cout << str << "\n";
}

void println(glm::mat4 mat) {
    std::cout << glm::to_string(mat) << "\n";
}

void println(std::string str, glm::mat4 mat) {
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




template <typename T>
void partheader(T header) {
    println("\n--------");
    println(header);
    println("--------");
}
#endif //INC_28_MODEL_LOADING_CPP_HELPER_H
