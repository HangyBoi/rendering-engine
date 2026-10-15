cmake_minimum_required(VERSION 3.29)
project(RawEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(imgui imgui/backends)

# Add source files for ImGui
set(IMGUI_SOURCES
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(RawEngine main.cpp
        ${IMGUI_SOURCES}
        core/assimpLoader.cpp
        core/assimpLoader.h
        core/mesh.cpp
        core/mesh.h
        core/vertex.h
        core/model.cpp
        core/model.h
        core/shader.cpp
        core/shader.h
        core/texture.cpp
        core/texture.h
        core/camera.cpp
        core/camera.h
)

add_custom_command(
        TARGET RawEngine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/)

add_custom_command(
        TARGET RawEngine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/models/
        ${CMAKE_CURRENT_BINARY_DIR}/models/)

add_custom_command(
        TARGET RawEngine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures/
        ${CMAKE_CURRENT_BINARY_DIR}/textures/)

# Repo change: this target runs on EVERY build and copies shaders, models and textures next to the
# executable. (The PRE_BUILD commands above only re-run when RawEngine itself is relinked, so an edited
# shader/model/texture would otherwise not reach the build folder without a code change.)
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_BINARY_DIR}/models
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures
        ${CMAKE_CURRENT_BINARY_DIR}/textures
        COMMENT "Copying shaders, models and textures..."
)

add_dependencies(RawEngine copy_assets)

find_package(glad CONFIG REQUIRED)
target_link_libraries(RawEngine PRIVATE glad::glad)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(RawEngine PRIVATE glfw)
find_package(glm CONFIG REQUIRED)
target_link_libraries(RawEngine PRIVATE glm::glm)
find_package(assimp CONFIG REQUIRED)
target_link_libraries(RawEngine PRIVATE assimp::assimp)
find_package(Stb REQUIRED)
target_include_directories(RawEngine PRIVATE ${Stb_INCLUDE_DIR})