cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(3DGraphics DESCRIPTION "3D Graphics interactive assignment, build system by Camille Ferrari")

set(PROJECT_ROOT .)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

include(GNUInstallDirs)

set(SRC_FILES
    ${PROJECT_ROOT}/Source.cpp
    ${PROJECT_ROOT}/src/Content.cpp
    ${PROJECT_ROOT}/src/Pipeline.cpp
    ${PROJECT_ROOT}/src/Debugger.cpp
)

set(SRC_LIBS
    ${PROJECT_ROOT}/src/glad.c # glad.c shouldn't be in src but the template has it here by default
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
    ${SRC_LIBS}
)

target_compile_options(3DGraphics
    PRIVATE -W -Wall -Wextra -Wno-missing-field-initializers
)

target_include_directories(3DGraphics
    PRIVATE
        ${PROJECT_ROOT}/include/
)

target_link_directories(3DGraphics
    PRIVATE
        ${PROJECT_ROOT}/lib/
)

target_link_libraries(3DGraphics opengl32 glu32 gdi32 libglfw3.a)
