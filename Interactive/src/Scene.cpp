#include "Scene.hpp"

void Scene::setup(GLApp::Context& ctx)
{
    cout << endl << "Loading content..." << endl;
    this->model.loadGLTF("./assets/helmet/DamagedHelmet.gltf");
    this->maxwell.loadGLTF("./assets/maxwell.gltf");

    this->maxwell.transform.scale = {.05, .05, .05};

    // this->shader.init("shaders/vs_model.glsl", "shaders/fs_model.glsl");
    this->shader.init("shaders/pbr_model.vert", "shaders/pbr_model.frag");

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

    // this->model.transform.rotation.y += .03f;

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

    this->shader.setVec3("view_pos", camera.Position);
    float t = glfwGetTime();
    light_pos = vec3(cos(t), light_pos.y, sin(t));

    // std::cout << t << std::endl;
    this->maxwell.transform.position = light_pos;

    this->shader.setVec3("light_pos[0]", light_pos);
    this->shader.setVec3("light_colors[0]", vec3(1.0, 1.0, 1.0));

    this->shader.setVec3("camera_pos", camera.Position);

    this->shader.setMat4("view_matrix", viewMatrix);
    this->shader.setMat4("proj_matrix", ctx.projMatrix);

    this->model.draw(this->shader);
    this->maxwell.draw(this->shader);

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

    ImGui::InputFloat3("Maxwell position", glm::value_ptr(light_pos));
    ImGui::End();

    // Rendering imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene::onResizeCallback(UNUSED GLFWwindow* window, UNUSED int w, UNUSED int h){}
void Scene::onKeyCallback(GLFWwindow* window, int key, UNUSED int scancode, int action, UNUSED int mods)
{
    GLApp::Context* ctx = static_cast<GLApp::Context*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        cursor = !cursor;
        glfwSetInputMode(ctx->window, GLFW_CURSOR, cursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}
void Scene::onMouseButtonCallback(UNUSED GLFWwindow* window, UNUSED int button, UNUSED int action, UNUSED int mods){}
void Scene::onMouseMoveCallback(UNUSED GLFWwindow* window, double x, double y)
{
    static double previousX = x, previousY = y;
    if (!cursor)
        this->camera.ProcessMouseMovement(x - previousX, previousY - y);
    previousX = x;
    previousY = y;
}
void Scene::onMouseWheelCallback(UNUSED GLFWwindow* window, UNUSED double xoffset, double yoffset)
{
    this->camera.ProcessMouseScroll(yoffset);
}