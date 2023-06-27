cmake_minimum_required(VERSION 3.19)
project(vulkanTutorial CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)

file(TO_CMAKE_PATH $ENV{VULKAN_SDK} VULKAN_PATH)
include_directories(glfw/glfw-3.2.1.bin.WIN64/include glm stb ${VULKAN_PATH}/Include src_ben)
SET(SOURCES GameObject.cpp BehaviourComponent.cpp deltaTime.cpp bounds.cpp)
add_executable(GAME gameMain.cpp ${SOURCES})


target_link_libraries(GAME ${VULKAN_PATH}/Lib/vulkan-1.lib)
add_library(glfw SHARED IMPORTED)
set_target_properties( glfw PROPERTIES IMPORTED_IMPLIB C:/UniProject/RCG_final/glfw/glfw-3.2.1.bin.WIN64/lib-mingw-w64/libglfw3dll.a)
set_target_properties( glfw PROPERTIES IMPORTED_LOCATION C:/UniProject/RCG_final/glfw/glfw-3.2.1.bin.WIN64/lib-mingw-w64/glfw3.dll)
target_link_libraries(GAME glfw)

# still not working sadge



