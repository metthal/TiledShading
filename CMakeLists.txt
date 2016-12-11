cmake_minimum_required(VERSION 3.1)
project(TiledShading CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dep/imgui-1.49")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

add_subdirectory(src)
add_subdirectory(dep)