#pragma once

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h> // Extend OpenGL Specfication to version 4.5 for WIN64 and versions 4.1 for Apple (note: two different files).

#include <imgui/imgui.h>			  // Load GUI library - Dear ImGui - https://github.com/ocornut/imgui
#include <imgui/imgui_impl_glfw.h>	  // Platform ImGui using GLFW
#include <imgui/imgui_impl_opengl3.h> // Platform new OpenGL - aka better than 3.3 core version.

#include <GLFW/glfw3.h> // Add GLFW - library to launch a window and callback mouse and keyboard - https://www.glfw.org

#define GLM_ENABLE_EXPERIMENTAL	 // Enable all functions include non LTS
#include <glm/glm.hpp>			 // Add helper maths library - GLM 0.9.9.9 - https://github.com/g-truc/glm - for example variables vec3, mat and operators.
#include <glm/gtx/transform.hpp> // Help us with transforms
using namespace glm;

#include "Debugger.hpp"

#include <vector>
#include <iostream>

#define UNUSED __attribute__((unused))

namespace GLApp
{
    enum Status {
        SUCCESS,
        FAILURE
    };

    class IScene;

    class Context
    {
        public:
            Context(void) = default;
            ~Context(void) = default;
            GLFWwindow* window;                                         // Keep track of the window
            int windowWidth = 800;                                      // Window width
            int windowHeight = 800;                                     // Window height
            float aspect = (float)windowWidth / (float)windowHeight;    // Window aspect ration
            mat4 projMatrix;                                            // Our Projection Matrix
            float fovy = 45.0f;                                         // Field of view (y axis)

            bool keyStatus[1024] = {0};                                 // Track key strokes
            float currentTime = 0.0f;                                   // Framerate
            float deltaTime = 0.0f;                                     // time passed
            float lastTime = 0.0f;                                      // Used to calculate Frame rate

            Status init(size_t defaultScene = 0);
            void destroy(void);
            void run(void);

            int addScene(IScene*);
            Status switchScene(size_t sceneId);

        private:
            std::vector<IScene*> scenes;
            size_t currentScene = 0;

            // OpenGL callbacks
            void hintsGLFW(void);
            static void onResizeCallback(GLFWwindow* window, int w, int h);
            static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            static void onMouseMoveCallback(GLFWwindow* window, double x, double y);
            static void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);
    };
};