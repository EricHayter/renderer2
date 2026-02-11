#include <filesystem>
#include <memory>

#include "debug_window.h"
#include "fps_tracker.h"
#include "scene.h"
#include "shader.h"
#include "window.h"

int main(int argc, const char** const argv) {
    Window window({});
    Scene scene;
    Shader shader = Shader("../shaders/vertex.vs", "../shaders/fragment.fs");

    // Load models from command line arguments
    for (int i = 1; i < argc; i++) {
        scene.model.push_back(std::make_unique<Model>(argv[i]));
    }

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
