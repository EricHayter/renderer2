#pragma once

// clang format will change the input order which MUST be in this specific
// order
// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <optional>
#include <string>

namespace window {

struct WindowConfig {
    int width = 800;
    int height = 600;
    std::string name = "renderer2";
};

// Initializes the GLFW window object. With parameters from the WindowConfig
// struct. If initialization fails return std::null_opt.
std::optional<GLFWwindow*> InitWindow(const WindowConfig& config);

// Called automatically by GLFW when the window is resized.
// Updates the OpenGL viewport to match the new window dimensions.
// This callback is registered automatically by InitWindow().
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void ProcessInput(GLFWwindow* window);

};
