#include "Scene.hpp"

void Scene::setup(GLApp::Context& ctx)
{
    cout << endl << "Loading content..." << endl;
    this->model.LoadGLTF("./assets/table.gltf");
    // this->model1.LoadGLTF("./assets/dog.gltf");

    this->model.transform.scale = {1.2, 1.2, 1.2};

    // this->pipeline.CreatePipeline();
    // this->pipeline.LoadShaders("shaders/vs_model.glsl", "shaders/fs_model.glsl");

    this->shader.init("shaders/vs_model.glsl", "shaders/fs_model.glsl");

    // Get the correct size in pixels - E.g. if retina display or monitor scaling
    glfwGetFramebufferSize(ctx.window, &ctx.windowWidth, &ctx.windowHeight);

    // Calculate proj_matrix for the first time.
    ctx.aspect = (float)ctx.windowWidth / (float)ctx.windowHeight;
    ctx.projMatrix = glm::perspective(glm::radians(ctx.fovy), ctx.aspect, 0.1f, 1000.0f);
    glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Scene::update(GLApp::Context& ctx)
{
    if (ctx.keyStatus[GLFW_KEY_LEFT]) this->model.transform.rotation.y += 0.05f;
    if (ctx.keyStatus[GLFW_KEY_RIGHT]) this->model.transform.rotation.y -= 0.05f;
    if (ctx.keyStatus[GLFW_KEY_UP]) this->model.transform.rotation.x += 0.05f;
    if (ctx.keyStatus[GLFW_KEY_DOWN]) this->model.transform.rotation.x -= 0.05f;
    // if (ctx.keyStatus[GLFW_KEY_W]) this->modelPosition.z += 0.10f;
    // if (ctx.keyStatus[GLFW_KEY_S]) this->modelPosition.z -= 0.10f;

    if (ctx.keyStatus[GLFW_KEY_W]) this->camera.ProcessKeyboard(GLApp::FORWARD, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_S]) this->camera.ProcessKeyboard(GLApp::BACKWARD, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_A]) this->camera.ProcessKeyboard(GLApp::LEFT, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_D]) this->camera.ProcessKeyboard(GLApp::RIGHT, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_Q]) this->camera.ProcessKeyboard(GLApp::DOWN, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_E]) this->camera.ProcessKeyboard(GLApp::UP, ctx.deltaTime);

    if (ctx.keyStatus[GLFW_KEY_R]) this->shader.reloadShaders();
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
    glm::mat4 viewMatrix = camera.GetViewMatrix();

    // Do some translations, rotations and scaling

    // Use our shader programs
    this->shader.use();

    this->shader.setMat4("view_matrix", viewMatrix);
    this->shader.setMat4("proj_matrix", ctx.projMatrix);

    this->model.Draw(this->shader);
    // this->model1.Draw();

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
        ImGui::Text("Pipeline: %s", this->shader.error ? "ERROR" : "OK");
    }
    ImGui::End();

    // Rendering imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene::onResizeCallback(UNUSED GLFWwindow* window, UNUSED int w, UNUSED int h){}
void Scene::onKeyCallback(UNUSED GLFWwindow* window, UNUSED int key, UNUSED int scancode, UNUSED int action, UNUSED int mods){}
void Scene::onMouseButtonCallback(UNUSED GLFWwindow* window, UNUSED int button, UNUSED int action, UNUSED int mods){}
void Scene::onMouseMoveCallback(UNUSED GLFWwindow* window, double x, double y)
{
    static double previousX = x, previousY = y;
    this->camera.ProcessMouseMovement(x - previousX, previousY - y);
    previousX = x;
    previousY = y;
}
void Scene::onMouseWheelCallback(UNUSED GLFWwindow* window, UNUSED double xoffset, double yoffset)
{
    this->camera.ProcessMouseScroll(yoffset);
}