#include "debug_window.h"

#include "glm/gtc/constants.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

DebugWindow::DebugWindow(Window& window, FPSTracker& fps_tracker)
    : window_m{window}, fps_tracker_m{fps_tracker} {
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
        window.GetGLFWWindow(),
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
    if (light_follows_camera_m) {
        scene.light.position = scene.camera.position;
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

    FPSMenu();
    CameraMenu(scene);
    LightingMenu(scene);
    ModelsMenu(scene);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugWindow::FPSMenu() {
    if (!ImGui::CollapsingHeader("Performance")) return;

    float fps = fps_tracker_m.GetFPS();
    ImGui::Text("FPS: %.1f", fps);

    if (fps > 0.0f) {
        float frame_time_ms = 1000.0f / fps;
        ImGui::Text("Frame Time: %.2f ms", frame_time_ms);
    }

    ImGui::Separator();
    if (ImGui::Checkbox("V-Sync", &vsync_enabled_m)) {
        window_m.SetVSync(vsync_enabled_m);
    }
}

void DebugWindow::CameraMenu(Scene& scene) {
    if (!ImGui::CollapsingHeader("Camera")) return;

    Camera& camera = scene.camera;

    // Position
    ImGui::Text("Position:");
    ImGui::Text("  X: %.2f", camera.position.x);
    ImGui::Text("  Y: %.2f", camera.position.y);
    ImGui::Text("  Z: %.2f", camera.position.z);

    ImGui::Separator();

    // Orientation (direction vector)
    ImGui::Text("Direction:");
    ImGui::Text("  X: %.2f", camera.pointing_at.x);
    ImGui::Text("  Y: %.2f", camera.pointing_at.y);
    ImGui::Text("  Z: %.2f", camera.pointing_at.z);

    ImGui::Separator();

    // Angles
    ImGui::Text("Angles:");
    ImGui::Text("  Yaw: %.1f°", camera.yaw);
    ImGui::Text("  Pitch: %.1f°", camera.pitch);

    ImGui::Separator();

    // FOV
    float fov_degrees = glm::degrees(camera.fov);
    ImGui::Text("FOV: %.1f°", fov_degrees);
}

void DebugWindow::LightingMenu(Scene& scene) {
    if (!ImGui::CollapsingHeader("Lighting")) return;

    ImGui::Checkbox("Follow Camera", &light_follows_camera_m);

    glm::vec3& light_pos = scene.light.position;
    if (light_follows_camera_m) {
        ImGui::BeginDisabled();
    }
    ImGui::DragFloat3("Light Position", &light_pos.x, 0.1f);
    if (light_follows_camera_m) {
        ImGui::EndDisabled();
    }

    Light& light = scene.light;
    ImGui::ColorEdit3("ambient", &light.ambient.x);
    ImGui::ColorEdit3("diffuse", &light.diffuse.x);
    ImGui::ColorEdit3("specular", &light.specular.x);
}

void DebugWindow::ModelsMenu(Scene& scene) {
    if (!ImGui::CollapsingHeader("Models")) return;

    Model& model = scene.model;

    ImGui::Text("Model ID: %d", model.GetID());
    ImGui::Separator();

    // Translation controls
    ImGui::DragFloat3("Translation", &model.translation.x, 0.1f);

    // Scale controls
    ImGui::DragFloat3("Scale", &model.scale.x, 0.01f, 0.001f, 100.0f);

    // Coordinate system toggle
    ImGui::Checkbox("Y-Up (unchecked = Z-Up)", &model.is_y_up);

    ImGui::Separator();
    ImGui::Text("Stats:");
    ImGui::Text("  Vertices: %zu", model.GetVertexCount());
    ImGui::Text("  Triangles: %zu", model.GetTriangleCount());
    ImGui::Text("  Meshes: %zu", model.GetMeshCount());
}
