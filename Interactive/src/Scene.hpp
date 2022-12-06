#pragma once

#include "GLApp/IScene.hpp"
#include "GLApp/Model.hpp"
#include "GLApp/Debugger.hpp"
#include "GLApp/Shader.hpp"
#include "GLApp/Camera.hpp"

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
        bool running = true;
        GLApp::Camera camera = GLApp::Camera(vec3(0.0f, 1.0f, 5.0f), vec3(0.0f, 1.0f, 0.0f));

        float currentTime = 0.0f;
        float deltaTime = 0.0f;
        float lastTime = 0.0f;
        bool cursor = false;

        bool planeWireframe = false;
        float stepValue = .5;
        float bumpStrength = .3;

        glm::vec3 light_pos = vec3(1, 1.0, 0);

        GLApp::Model model;
        GLApp::Model maxwell;
        GLApp::Model plane;

        GLApp::Shader PBRShader;
        GLApp::Shader unlitShader;
        GLApp::Shader planeShader;

        GLApp::Debugger debugger;

        vec3 modelPosition;
        vec3 modelRotation;

};