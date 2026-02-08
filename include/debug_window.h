#pragma once

// clang format will change the input order which MUST be in this specific
// order
// clang-format off
#include "glad/gl.h"
#include "GLFW/glfw3.h"
// clang-format on

#include "scene.h"
#include "fps_tracker.h"

class DebugWindow {
    public:
    DebugWindow(GLFWwindow* window, FPSTracker& fps_tracker);
    ~DebugWindow();
    void Draw(Scene& scene);

    void FPSMenu();
    void LightingMenu(Scene& scene);
    void ModelsMenu(Scene& scene);

    private:
    bool light_follows_camera_m = false;
    FPSTracker& fps_tracker_m;
};
