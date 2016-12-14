cmake_minimum_required(VERSION 3.1)
project(TiledShading CXX)

macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dep/imgui-1.49")

set(MODELS_DIR "${CMAKE_SOURCE_DIR}/models")
set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(TEXTURES_DIR "${CMAKE_SOURCE_DIR}/textures")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_subdirectory(src)
add_subdirectory(dep)