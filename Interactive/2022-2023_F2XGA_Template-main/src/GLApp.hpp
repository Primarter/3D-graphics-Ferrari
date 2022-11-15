#ifndef GLAPP
#define GLAPP

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

#include "src/Pipeline.hpp"         // Setup pipeline and load shaders.
#include "src/Content.hpp"          // Setup content loader and drawing functions - https://github.com/KhronosGroup/glTF - https://github.com/syoyo/tinygltf
#include "src/Debugger.hpp"         // Setup debugger functions.

namespace GLApp
{
    GLFWwindow *window;                                         // Keep track of the window
    int windowWidth = 800;                                      // Window width
    int windowHeight = 800;                                     // Window height
    bool running = true;                                        // Are we still running our main loop
    mat4 projMatrix;                                            // Our Projection Matrix
    vec3 cameraPosition = vec3(0.0f, 0.0f, 5.0f);               // Where is our camera
    vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);                 // Camera front vector
    vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);                     // Camera up vector
    float aspect = (float)windowWidth / (float)windowHeight;    // Window aspect ration
    float fovy = 45.0f;                                         // Field of view (y axis)
    bool keyStatus[1024];                                       // Track key strokes
    float currentTime = 0.0f;                                   // Framerate
    float deltaTime = 0.0f;                                     // time passed
    float lastTime = 0.0f;                                      // Used to calculate Frame rate

    Pipeline pipeline;                                          // Add one pipeline plus some shaders.
    Content content;                                            // Add one content loader (+drawing).
    Debugger debugger;                                          // Add one debugger to use for callbacks ( Win64 - openGLDebugCallback() ) or manual calls ( Apple - glCheckError() )

    vec3 modelPosition;                                         // Model position
    vec3 modelRotation;                                         // Model rotation

    enum Status {
        SUCCESS,
        FAILURE
    };

    Status init(void);

    // OpenGL callbacks
    void onResizeCallback(GLFWwindow *window, int w, int h);
    void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void onMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void onMouseMoveCallback(GLFWwindow *window, double x, double y);
    void onMouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset);
}

#endif