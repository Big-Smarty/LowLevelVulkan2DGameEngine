cmake_minimum_required(VERSION 3.7)
project(LowLevelVulkan2DGame)

find_package(glfw3 REQUIRED)
include_directories(LowLevelVulkan2DGame)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fmodules-ts)

add_executable(LowLevelVulkan2DGame source_files/main.cpp)
target_link_libraries(LowLevelVulkan2DGame glfw)