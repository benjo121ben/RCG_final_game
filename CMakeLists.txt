cmake_minimum_required(VERSION 3.19)
project(vulkanTutorial CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)

add_executable(TEST testMain.cpp)
target_include_directories(TEST PRIVATE glfw/glfw-3.2.1.bin.WIN64/include glm stb C:/VulkanSDK/1.3.239.0/Include)

file(TO_CMAKE_PATH $ENV{VULKAN_SDK} VULKAN_PATH)

target_link_libraries(TEST ${VULKAN_PATH}/Lib/vulkan-1.lib)
add_library(glfw SHARED IMPORTED)
set_target_properties( glfw PROPERTIES IMPORTED_IMPLIB F:/UniProject/RCG/VulkanTutorial/glfw/glfw-3.2.1.bin.WIN64/lib-vc2015/glfw3dll.lib)
set_target_properties( glfw PROPERTIES IMPORTED_LOCATION F:/UniProject/RCG/VulkanTutorial/glfw/glfw-3.2.1.bin.WIN64/lib-vc2015/glfw3.dll)
target_link_libraries(TEST glfw)

# still not working sadge



