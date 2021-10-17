cmake_minimum_required(VERSION 3.7)
project(LowLevelVulkan2DGameEngine)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

include_directories(LowLevelVulkan2DGameEngine)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fmodules-ts -lz)

add_executable(LowLevelVulkan2DGameEngine source_files/main.cpp header_files/VkSetup.h)
target_link_libraries(LowLevelVulkan2DGameEngine glfw Vulkan::Vulkan)