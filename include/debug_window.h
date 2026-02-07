#pragma once

// clang format will change the input order which MUST be in this specific
// order
// clang-format off
#include "glad/gl.h"
#include "GLFW/glfw3.h"
// clang-format on

#include "scene.h"

class DebugWindow {
    public:
    DebugWindow(GLFWwindow* window);
    ~DebugWindow();
    void Draw(Scene& scene);

    void LightingMenu(Scene& scene);
};
