cmake_minimum_required(VERSION 3.7)
project(LowLevelVulkan2DGame)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

include_directories(LowLevelVulkan2DGame)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fmodules-ts -lz)

add_executable(LowLevelVulkan2DGame source_files/main.cpp header_files/VkSetup.h)
target_link_libraries(LowLevelVulkan2DGame glfw Vulkan::Vulkan)