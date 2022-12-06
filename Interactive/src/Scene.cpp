#include "Scene.hpp"

void Scene::setup(GLApp::Context& ctx)
{
    cout << endl << "Loading content..." << endl;

    GLApp::FeatureMask mask = GLApp::NONE | GLApp::ALBEDO | GLApp::NORMALS | GLApp::METALLIC_ROUGHNESS;
    this->model.loadGLTF("./assets/helmet/DamagedHelmet.gltf", mask);

    mask = GLApp::NONE | GLApp::ALBEDO;
    this->maxwell.loadGLTF("./assets/maxwell/maxwell.gltf", mask);
    this->maxwell.transform.scale = {.05, .05, .05};

    mask = GLApp::NONE;
    this->plane.loadGLTF("./assets/plane/plane.gltf", mask);
    this->plane.transform.position.y = -1;
    this->plane.transform.scale = {20, 1, 20};

    this->unlitShader.init("shaders/unlit_model.vert", "shaders/unlit_model.frag");
    this->PBRShader.init("shaders/pbr_model.vert", "shaders/pbr_model.frag");
    this->planeShader.init("shaders/plane.vert", "shaders/plane.frag");

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

    this->model.transform.rotation.y -= .005f;

    if (ctx.keyStatus[GLFW_KEY_W]) this->camera.ProcessKeyboard(GLApp::FORWARD, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_S]) this->camera.ProcessKeyboard(GLApp::BACKWARD, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_A]) this->camera.ProcessKeyboard(GLApp::LEFT, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_D]) this->camera.ProcessKeyboard(GLApp::RIGHT, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_Q]) this->camera.ProcessKeyboard(GLApp::DOWN, ctx.deltaTime);
    if (ctx.keyStatus[GLFW_KEY_E]) this->camera.ProcessKeyboard(GLApp::UP, ctx.deltaTime);

    if (ctx.keyStatus[GLFW_KEY_R]) this->PBRShader.reloadShaders();
}

void Scene::render(GLApp::Context& ctx)
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, ctx.windowWidth, ctx.windowHeight);

    // Clear colour buffer
    glm::vec4 inchyraBlue = glm::vec4(0.345f, 0.404f, 0.408f, 1.0f);
    glm::vec4 backgroundColor = inchyraBlue;
    glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

    // Clear depth buffer
    static const GLfloat one = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &one);

    // Specify blend function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 viewMatrix = camera.GetViewMatrix();

    float t = glfwGetTime();
    light_pos = vec3(cos(t), light_pos.y, sin(t));


    this->PBRShader.use();
    this->PBRShader.setVec3("light_pos[0]", light_pos);
    this->PBRShader.setVec3("light_colors[0]", vec3(1.0, 1.0, 1.0));
    this->PBRShader.setVec3("camera_pos", camera.Position);
    this->PBRShader.setMat4("view_matrix", viewMatrix);
    this->PBRShader.setMat4("proj_matrix", ctx.projMatrix);
    this->model.draw(this->PBRShader);

    this->maxwell.transform.rotation.y = t * 2.0;
    this->maxwell.transform.position = light_pos;
    this->unlitShader.use();
    this->unlitShader.setMat4("view_matrix", viewMatrix);
    this->unlitShader.setMat4("proj_matrix", ctx.projMatrix);
    this->maxwell.draw(this->unlitShader);

    if (planeWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    this->planeShader.use();
    this->planeShader.setMat4("view_matrix", viewMatrix);
    this->planeShader.setMat4("proj_matrix", ctx.projMatrix);
    this->planeShader.setFloat("step_value", stepValue);
    this->planeShader.setFloat("bump_strength", bumpStrength);
    this->planeShader.setFloat("time", t);
    this->plane.draw(this->planeShader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    #if defined(__APPLE__)
        glCheckError();
    #endif
}

void Scene::ui(UNUSED GLApp::Context& ctx)
{
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;
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
        ImGui::Text("3D Graphics and Animation 2022, Camille Ferrari"); ImGui::Separator();
        ImGui::Text("Performance: %.3fms/Frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Pipeline: %s", this->PBRShader.error ? "ERROR" : "OK");
    }

    ImGui::InputFloat3("Maxwell position", glm::value_ptr(light_pos));
    ImGui::Checkbox("Plane wireframe", &planeWireframe);
    ImGui::SliderFloat("Step value", &stepValue, 0.0, 1.0, "%.3f");
    ImGui::SliderFloat("Bump value", &bumpStrength, 0.0, 1.0, "%.3f");
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