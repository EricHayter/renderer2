#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "scene.h"
#include "shader.h"
#include "window.h"

int main(int argc, const char** const argv) {
    if (argc != 2) {
        std::cout << std::format("Usage: {} FILE\n", argv[0]);
        return -1;
    }
    std::filesystem::path model_path = argv[1];
    if (!std::filesystem::exists(model_path)) {
        std::cout << std::format("Could'nt find model at '{}'\n",
                                 model_path.string());
        return -1;
    }

    Window window({});

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
        window.GetWindow(),
        true);  // Second param install_callback=true will install GLFW
                // callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    Scene scene = Scene(model_path);
    Shader shader = Shader("../shaders/vertex.vs", "../shaders/fragment.fs");

    glEnable(GL_DEPTH_TEST);

    // For tracking FPS
    constexpr int FPS_REPORT_INTERVAL_MS = 5000;
    int frames_displayed = 0;
    auto past_time = std::chrono::steady_clock::now();

    //    // Model stats
    //    size_t vertex_count = model.GetVertexCount();
    //    size_t triangle_count = model.GetTriangleCount();
    //    size_t mesh_count = model.GetMeshCount();

    // Render loop
    while (!window.ShouldClose()) {
        // handling input
        window.ProcessInput(scene.GetCamera());
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        // Rendering

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        scene.Draw(shader, window);

        // swap buffers and poll for IO events
        window.SwapBuffers();
        window.PollEvents();

        // Calculating FPS
        //        frames_displayed++;
        //        auto current_time = std::chrono::steady_clock::now();
        //        int ms_elapsed =
        //        std::chrono::duration_cast<std::chrono::milliseconds>(
        //                             current_time - past_time)
        //                             .count();
        //        if (ms_elapsed > FPS_REPORT_INTERVAL_MS) {
        //            float fps = frames_displayed / (ms_elapsed / 1000.0f);
        //            std::cout << std::format(
        //                "[renderer2] {:.1f} FPS | {} vertices | {} triangles |
        //                {} " "meshes\n", fps, vertex_count, triangle_count,
        //                mesh_count);
        //            frames_displayed = 0;
        //            past_time = std::chrono::steady_clock::now();
        //        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
