file(GLOB_RECURSE SOURCES *.cpp *.h)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SYSTEM ${OPENGL_INCLUDE_DIR}
    SYSTEM ${GLEW_INCLUDE_DIR}
    SYSTEM ${SDL2_INCLUDE_DIR}
    SYSTEM ${IMGUI_INCLUDE_DIR}
    SYSTEM ${GLM_INCLUDE_DIR}
    SYSTEM ${ASSIMP_INCLUDE_DIR}
)

add_executable(TiledShading ${SOURCES})
target_link_libraries(TiledShading ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY} ${SDL2_LIBRARY} ${ASSIMP_LIBRARY} imgui)

add_custom_target(
    always_copy_resources ALL
    DEPENDS COPY_RESOURCES
)

if(MSVC)
    add_custom_command(
        OUTPUT COPY_RESOURCES
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLEW_RUNTIME_LIBRARY} $<TARGET_FILE_DIR:TiledShading>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_RUNTIME_LIBRARY} $<TARGET_FILE_DIR:TiledShading>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSIMP_RUNTIME_LIBRARY} $<TARGET_FILE_DIR:TiledShading>
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODELS_DIR} $<TARGET_FILE_DIR:TiledShading>/models
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_DIR} $<TARGET_FILE_DIR:TiledShading>/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURES_DIR} $<TARGET_FILE_DIR:TiledShading>/textures
    )
    
    subdirlist(DIRS ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(SUBDIR ${DIRS})
        source_group("Source Files\\${SUBDIR}" REGULAR_EXPRESSION "${SUBDIR}/[a-z_]*\.cpp$")
        source_group("Header Files\\${SUBDIR}" REGULAR_EXPRESSION "${SUBDIR}/[a-z_]*\.h$")
    endforeach()
else()
    add_custom_command(
        OUTPUT COPY_RESOURCES
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODELS_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURES_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()

set_property(TARGET TiledShading PROPERTY CXX_STANDARD 14)