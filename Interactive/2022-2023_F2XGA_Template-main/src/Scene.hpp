#pragma once

#include "GLApp/IScene.hpp"
#include "GLApp/Model.hpp"
#include "GLApp/Pipeline.hpp"
#include "GLApp/Debugger.hpp"

class Scene : public GLApp::IScene {

    public:
        Scene(void) = default;
        ~Scene(void) = default;

        void setup(GLApp::Context&) final;
        void update(GLApp::Context&) final;
        void render(GLApp::Context&) final;
        void ui(GLApp::Context&) final;

        void onResizeCallback(GLFWwindow* window, int w, int h) final;
        void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) final;
        void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) final;
        void onMouseMoveCallback(GLFWwindow* window, double x, double y) final;
        void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) final;

        int sceneId = 0;
        bool running = true;                                        // Are we still running our main loop
        vec3 cameraPosition = vec3(0.0f, 0.0f, 5.0f);               // Where is our camera
        vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);                 // Camera front vector
        vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);                     // Camera up vector
        float currentTime = 0.0f;                                   // Framerate
        float deltaTime = 0.0f;                                     // time passed
        float lastTime = 0.0f;                                      // Used to calculate Frame rate

        GLApp::Pipeline pipeline;                                          // Pipeline plus some shaders.
        GLApp::Model content;                                              // Content loader (+drawing).
        GLApp::Debugger debugger;                                          // Debugger to use for callbacks ( Win64 - openGLDebugCallback() ) or manual calls ( Apple - glCheckError() )

        vec3 modelPosition;                                         // Model position
        vec3 modelRotation;                                         // Model rotation

};