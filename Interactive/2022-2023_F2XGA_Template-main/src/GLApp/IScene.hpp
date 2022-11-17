#pragma once

#include "GLApp.hpp"

namespace GLApp {
    class IScene
    {
        public:
            virtual void setup(GLApp::Context&) = 0;
            virtual void update(GLApp::Context&) = 0;
            virtual void render(GLApp::Context&) = 0;
            virtual void ui(UNUSED GLApp::Context&) {};

            virtual void onResizeCallback(UNUSED GLFWwindow* window, UNUSED int w, UNUSED int h) {};
            virtual void onKeyCallback(UNUSED GLFWwindow* window, UNUSED int key, UNUSED int scancode, UNUSED int action, UNUSED int mods) {};
            virtual void onMouseButtonCallback(UNUSED GLFWwindow* window, UNUSED int button, UNUSED int action, UNUSED int mods) {};
            virtual void onMouseMoveCallback(UNUSED GLFWwindow* window, UNUSED double x, UNUSED double y) {};
            virtual void onMouseWheelCallback(UNUSED GLFWwindow* window, UNUSED double xoffset, UNUSED double yoffset) {};
    };
}