#include "GLApp.hpp"

namespace GLApp
{
    void hintsGLFW()
    {

        auto majorVersion = 3; auto minorVersion = 3; // define OpenGL version - at least 3.3 for bare basic NEW OpenGL

        #if defined(__WIN64__)
            majorVersion = 4; minorVersion = 5;                    // Recommended for Windows 4.5, but latest is 4.6 (not all drivers support 4.6 version).
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // Create context in debug mode - for debug message callback
        #elif(__APPLE__)
            majorVersion = 4; minorVersion = 1;                 // Max support for OpenGL in MacOS
        #endif

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);     // https://www.glfw.org/docs/3.3/window_guide.html
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    }

    Status init(void)
    {
        cout << endl << "===" << endl << "3D Graphics and Animation - Running..." << endl;

        if (!glfwInit()) // Check if GLFW is working, if not then stop all
        {
            cout << "Could not initialise GLFW...";
            return FAILURE;
        }

        glfwSetErrorCallback(errorCallbackGLFW); // Setup a function callback to catch and display all GLFW errors.
        hintsGLFW();                             // Setup glfw with various hints.

        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor()); // Grab reference to monitor
        // windowWidth = mode->width; windowHeight = mode->height; //fullscreen
        // window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), glfwGetPrimaryMonitor(), NULL); // fullscreen

        // Create our Window
        const auto windowTitle = "My 3D Graphics and Animation OpenGL Application";
        window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
        if (!window) // Test if Window or OpenGL context creation failed
        {
            cout << "Could not initialise GLFW...";
            glfwTerminate();
            return FAILURE;
        }

        // glfwSetWindowPos(window, 30, 30); // Place it in top corner for easy debugging.
        glfwMakeContextCurrent(window);      // making the OpenGL context current

        // GLAD: Load OpenGL function pointers - aka update specs to newest versions - plus test for errors.
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD...";
            glfwMakeContextCurrent(NULL);
            glfwTerminate();
            return FAILURE;
        }

        glfwSetWindowSizeCallback(window, onResizeCallback);            // Set callback for resize
        glfwSetKeyCallback(window, onKeyCallback);                      // Set Callback for keys
        glfwSetMouseButtonCallback(window, onMouseButtonCallback);      // Set callback for mouse click
        glfwSetCursorPosCallback(window, onMouseMoveCallback);          // Set callback for mouse move
        glfwSetScrollCallback(window, onMouseWheelCallback);            // Set callback for mouse wheel.
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);   // Normal cursor (visible)
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Invisible cursor

        // Setup Dear ImGui and add context    -    https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Platform/Renderer ImGui backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        const auto glsl_version = "#version 410";
        ImGui_ImplOpenGL3_Init(glsl_version);

        #if defined(__WIN64__)
            debugGL(); // Setup callback to catch openGL errors.    V4.2 or newer
        #elif(__APPLE__)
            glCheckError(); // Old way of checking for errors. Newest not implemented by Apple. Manually call function anywhere in code to check for errors.
        #endif

        glfwSwapInterval(1);             // Ony render when synced (V SYNC) - https://www.tomsguide.com/features/what-is-vsync-and-should-you-turn-it-on-or-off
        // glfwWindowHint(GLFW_SAMPLES, 2); // Multisampling - covered in lectures - https://www.khronos.org/opengl/wiki/Multisampling
    }

    void onResizeCallback(GLFWwindow *window, int w, int h)
    {
        windowWidth = w;
        windowHeight = h;

        // Get the correct size in pixels
        // glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

        if (windowWidth > 0 && windowHeight > 0)
        { // Avoid issues when minimising window - it gives size of 0 which fails division.
            aspect = (float)w / (float)h;
            projMatrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
        }
    }

    void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        keyStatus[key] = action; // action == 1 if press, 0 if released
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void onMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
    }

    void onMouseMoveCallback(GLFWwindow *window, double x, double y)
    {
        int mouseX = static_cast<int>(x);
        int mouseY = static_cast<int>(y);
    }

    void onMouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        int yoffsetInt = static_cast<int>(yoffset);
    }

}