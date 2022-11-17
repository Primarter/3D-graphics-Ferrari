#include "Scene.hpp"

void Scene::setup(GLApp::Context& ctx)
{
    cout << endl << "Loading content..." << endl;
    this->content.LoadGLTF("./assets/dog.gltf");

    this->pipeline.CreatePipeline();
    this->pipeline.LoadShaders("shaders/vs_model.glsl", "shaders/fs_model.glsl");

    // Start from the centre
    this->modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    this->modelRotation = glm::vec3(0.0f, 0.0f, 0.0f);


    // Get the correct size in pixels - E.g. if retina display or monitor scaling
    glfwGetFramebufferSize(ctx.window, &ctx.windowWidth, &ctx.windowHeight);

    // Calculate proj_matrix for the first time.
    ctx.aspect = (float)ctx.windowWidth / (float)ctx.windowHeight;
    ctx.projMatrix = glm::perspective(glm::radians(ctx.fovy), ctx.aspect, 0.1f, 1000.0f);
}

void Scene::update(GLApp::Context& ctx)
{
    if (ctx.keyStatus[GLFW_KEY_LEFT]) this->modelRotation.y += 0.05f;
    if (ctx.keyStatus[GLFW_KEY_RIGHT]) this->modelRotation.y -= 0.05f;
    if (ctx.keyStatus[GLFW_KEY_UP]) this->modelRotation.x += 0.05f;
    if (ctx.keyStatus[GLFW_KEY_DOWN]) this->modelRotation.x -= 0.05f;
    if (ctx.keyStatus[GLFW_KEY_W]) this->modelPosition.z += 0.10f;
    if (ctx.keyStatus[GLFW_KEY_S]) this->modelPosition.z -= 0.10f;

    if (ctx.keyStatus[GLFW_KEY_R]) this->pipeline.ReloadShaders();
}

void Scene::render(GLApp::Context& ctx)
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, ctx.windowWidth, ctx.windowHeight);

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
    glm::mat4 viewMatrix = glm::lookAt(this->cameraPosition,                 // eye
                                       this->cameraPosition + this->cameraFront, // centre
                                       this->cameraUp);                     // up

    // Do some translations, rotations and scaling
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->modelPosition.x, this->modelPosition.y, this->modelPosition.z));
    // glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, this->modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, this->modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));

    // Use our shader programs
    glUseProgram(this->pipeline.pipe.program);

    glUniformMatrix4fv(glGetUniformLocation(this->pipeline.pipe.program, "model_matrix"), 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(this->pipeline.pipe.program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(this->pipeline.pipe.program, "proj_matrix"), 1, GL_FALSE, &ctx.projMatrix[0][0]);

    this->content.Draw(this->content.vaoAndEbos, this->content.model);

    #if defined(__APPLE__)
        glCheckError();
    #endif
}

void Scene::ui(UNUSED GLApp::Context& ctx)
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
    if (ImGui::Begin("Info", nullptr, window_flags)) {
        ImGui::Text("About: 3D Graphics and Animation 2022"); ImGui::Separator();
        ImGui::Text("Performance: %.3fms/Frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Pipeline: %s", this->pipeline.pipe.error?"ERROR":"OK");
    }
    ImGui::End();

    // Rendering imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene::onResizeCallback(UNUSED GLFWwindow* window, UNUSED int w, UNUSED int h){}
void Scene::onKeyCallback(UNUSED GLFWwindow* window, UNUSED int key, UNUSED int scancode, UNUSED int action, UNUSED int mods){}
void Scene::onMouseButtonCallback(UNUSED GLFWwindow* window, UNUSED int button, UNUSED int action, UNUSED int mods){}
void Scene::onMouseMoveCallback(UNUSED GLFWwindow* window, UNUSED double x, UNUSED double y){}
void Scene::onMouseWheelCallback(UNUSED GLFWwindow* window, UNUSED double xoffset, UNUSED double yoffset){}