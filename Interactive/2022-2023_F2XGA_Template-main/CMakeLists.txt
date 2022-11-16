cmake_minimum_required(VERSION 3.9)
project(3DGraphics DESCRIPTION "test project")

set(PROJECT_ROOT ./)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include(GNUInstallDirs)

set(SRC_FILES
    ${PROJECT_ROOT}/main.cpp
    ${PROJECT_ROOT}/src/Camera.cpp
    ${PROJECT_ROOT}/src/Debugger.cpp
    ${PROJECT_ROOT}/src/glad.c
    ${PROJECT_ROOT}/src/GLApp.cpp
    ${PROJECT_ROOT}/src/Model.cpp
    ${PROJECT_ROOT}/src/Pipeline.cpp
    ${PROJECT_ROOT}/src/Shader.cpp

    ${PROJECT_ROOT}/include/imgui/imgui.cpp
    ${PROJECT_ROOT}/include/imgui/imgui_demo.cpp
    ${PROJECT_ROOT}/include/imgui/imgui_draw.cpp
    ${PROJECT_ROOT}/include/imgui/imgui_widgets.cpp
    ${PROJECT_ROOT}/include/imgui/imgui_tables.cpp
    ${PROJECT_ROOT}/include/imgui/imgui_impl_opengl3.cpp
    ${PROJECT_ROOT}/include/imgui/imgui_impl_glfw.cpp
    ${PROJECT_ROOT}/include/tinygltf/tiny_gltf.cc
)

add_executable(3DGraphics
    ${SRC_FILES}
)

target_compile_options(3DGraphics PRIVATE -W -Wall -Wextra -Wno-missing-field-initializers -std=c++2a -L./lib)

target_include_directories(3DGraphics
    PRIVATE
        ${PROJECT_ROOT}/include/
)

target_link_libraries(3DGraphics glfw3 opengl32 glu32 gdi32)
