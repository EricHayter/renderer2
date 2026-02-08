#include "debug_window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

DebugWindow::DebugWindow(GLFWwindow* window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(
        window,
        true);  // Second param install_callback=true will install GLFW
                // callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

DebugWindow::~DebugWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugWindow::Draw(Scene& scene) {
    // Update light position if following camera
    if (light_follows_camera) {
        scene.GetLightPosition() = scene.GetCamera().position;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (!ImGui::Begin("renderer2")) {
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        return;
    }

    LightingMenu(scene);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugWindow::LightingMenu(Scene& scene) {
    if (!ImGui::CollapsingHeader("Lighting")) return;

    ImGui::Checkbox("Follow Camera", &light_follows_camera);

    glm::vec3& light_pos = scene.GetLightPosition();
    if (light_follows_camera) {
        ImGui::BeginDisabled();
    }
    ImGui::DragFloat3("Light Position", &light_pos.x, 0.1f);
    if (light_follows_camera) {
        ImGui::EndDisabled();
    }

    glm::vec3& light_color = scene.GetLightColor();
    ImGui::ColorEdit3("Light Color", &light_color.x);
}
