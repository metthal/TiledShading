file(GLOB_RECURSE SOURCES *.cpp)

include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
include_directories(SYSTEM ${GLEW_INCLUDE_DIR})
include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
include_directories(SYSTEM ${IMGUI_INCLUDE_DIR})

add_executable(TiledShading ${SOURCES})
target_link_libraries(TiledShading ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY} ${SDL2_LIBRARY} imgui)

if(MSVC)
    add_custom_command(
        TARGET TiledShading
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLEW_RUNTIME_LIBRARY} $<TARGET_FILE_DIR:TiledShading>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_RUNTIME_LIBRARY} $<TARGET_FILE_DIR:TiledShading>)
endif()

set_property(TARGET TiledShading PROPERTY CXX_STANDARD 14)