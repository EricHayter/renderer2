#include <filesystem>
#include <format>
#include <iostream>

#include "debug_window.h"
#include "fps_tracker.h"
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
        std::cout << std::format("Couldn't find model at '{}'\n",
                                 model_path.string());
        return -1;
    }

    Window window({});
    Scene scene = Scene(model_path);
    Shader shader = Shader("../shaders/vertex.vs", "../shaders/fragment.fs");

    glEnable(GL_DEPTH_TEST);

    FPSTracker fps_tracker;
    DebugWindow debug_window(window, fps_tracker);

    // Render loop
    while (!window.ShouldClose()) {
        // handling input
        window.ProcessInput(scene.camera);

        scene.Draw(shader, window);
        debug_window.Draw(scene);

        // swap buffers and poll for IO events
        window.SwapBuffers();
        window.PollEvents();
        fps_tracker.RecordFrame();
    }

    return 0;
}
