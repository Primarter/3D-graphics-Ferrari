// 3D Graphics and Animation - Main Template
// This uses Visual Studio Code - https://code.visualstudio.com/docs/cpp/introvideos-cpp
// Two versions available -  Win64 and Apple MacOS - please see notes
// Last changed August 2022

#include "src/GLApp.hpp"

// Main fuctions
void setup();
void update();
void render();
void ui();

int main()
{
    if (GLApp::init() == GLApp::FAILURE)
        return 1;

    setup(); // Setup all necessary information for startup (aka. load texture, shaders, models, etc).

    cout << endl << "Starting main loop and rendering..." << endl;

    while (!glfwWindowShouldClose(GLApp::window)) {                                             // run until the window is closed
        GLApp::updateTime();

        glfwPollEvents();                           // poll callbacks

        update();                                   // update (physics, animation, structures, etc)
        render();                                   // call render function.
        ui();                                       // call function to render ui.

        #if defined(__APPLE__)
            glCheckError();                         // Manually checking for errors for MacOS, Windows has a callback.
        #endif

        glfwSwapBuffers(GLApp::window);                    // swap buffers (avoid flickering and tearing)

    }

    GLApp::destroy(); // Close and clean everything up...

    return 0;
}

void setup()
{
    cout << endl << "Loading content..." << endl;
    GLApp::content.LoadGLTF("./assets/dog.gltf");

    GLApp::pipeline.CreatePipeline();
    GLApp::pipeline.LoadShaders("shaders/vs_model.glsl", "shaders/fs_model.glsl");

    // Start from the centre
    GLApp::modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    GLApp::modelRotation = glm::vec3(0.0f, 0.0f, 0.0f);


    // Get the correct size in pixels - E.g. if retina display or monitor scaling
    glfwGetFramebufferSize(GLApp::window, &GLApp::windowWidth, &GLApp::windowHeight);

    // Calculate proj_matrix for the first time.
    GLApp::aspect = (float)GLApp::windowWidth / (float)GLApp::windowHeight;
    GLApp::projMatrix = glm::perspective(glm::radians(GLApp::fovy), GLApp::aspect, 0.1f, 1000.0f);
}

void update()
{
    if (GLApp::keyStatus[GLFW_KEY_LEFT]) GLApp::modelRotation.y += 0.05f;
    if (GLApp::keyStatus[GLFW_KEY_RIGHT]) GLApp::modelRotation.y -= 0.05f;
    if (GLApp::keyStatus[GLFW_KEY_UP]) GLApp::modelRotation.x += 0.05f;
    if (GLApp::keyStatus[GLFW_KEY_DOWN]) GLApp::modelRotation.x -= 0.05f;
    if (GLApp::keyStatus[GLFW_KEY_W]) GLApp::modelPosition.z += 0.10f;
    if (GLApp::keyStatus[GLFW_KEY_S]) GLApp::modelPosition.z -= 0.10f;

    if (GLApp::keyStatus[GLFW_KEY_R]) GLApp::pipeline.ReloadShaders();
}

void render()
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, GLApp::windowWidth, GLApp::windowHeight);

    // Clear colour buffer
    glm::vec4 inchyraBlue = glm::vec4(0.345f, 0.404f, 0.408f, 1.0f);
    glm::vec4 backgroundColor = inchyraBlue;
    glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

    // Clear deep buffer
    static const GLfloat one = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &one);

    // Enable blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup camera
    glm::mat4 viewMatrix = glm::lookAt(GLApp::cameraPosition,                 // eye
                                       GLApp::cameraPosition + GLApp::cameraFront, // centre
                                       GLApp::cameraUp);                     // up

    // Do some translations, rotations and scaling
    // glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(GLApp::modelPosition.x+rX, GLApp::modelPosition.y+rY, GLApp::modelPosition.z+rZ));
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // modelMatrix = glm::rotate(modelMatrix, GLApp::modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, GLApp::modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));

    // Use our shader programs
    glUseProgram(GLApp::pipeline.pipe.program);

    glUniformMatrix4fv(glGetUniformLocation(GLApp::pipeline.pipe.program, "model_matrix"), 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(GLApp::pipeline.pipe.program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(GLApp::pipeline.pipe.program, "proj_matrix"), 1, GL_FALSE, &GLApp::projMatrix[0][0]);

    GLApp::content.Draw(GLApp::content.vaoAndEbos, GLApp::content.model);

    #if defined(__APPLE__)
        glCheckError();
    #endif
}

void ui()
{
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    window_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
    window_flags |= ImGuiWindowFlags_NoNav;

    const auto PAD = 10.0f;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = work_pos.x + work_size.x - PAD;
    window_pos.y = work_pos.y + work_size.y - PAD;
    window_pos_pivot.x = 1.0f;
    window_pos_pivot.y = 1.0f;

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    window_flags |= ImGuiWindowFlags_NoMove;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    bool *p_open = NULL;
    if (ImGui::Begin("Info", nullptr, window_flags)) {
        ImGui::Text("About: 3D Graphics and Animation 2022"); ImGui::Separator();
        ImGui::Text("Performance: %.3fms/Frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Pipeline: %s", GLApp::pipeline.pipe.error?"ERROR":"OK");
    }
    ImGui::End();

    // Rendering imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void onResizeCallback(GLFWwindow *window, int w, int h)
{
    GLApp::windowWidth = w;
    GLApp::windowHeight = h;

    // Get the correct size in pixels
    // glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    if (GLApp::windowWidth > 0 && GLApp::windowHeight > 0)
    { // Avoid issues when minimising window - it gives size of 0 which fails division.
        GLApp::aspect = (float)w / (float)h;
        GLApp::projMatrix = glm::perspective(glm::radians(GLApp::fovy), GLApp::aspect, 0.1f, 1000.0f);
    }
}

void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GLApp::keyStatus[key] = action; // action == 1 if press, 0 if released
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(GLApp::window, GLFW_TRUE);
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
