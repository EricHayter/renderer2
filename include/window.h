#pragma once

// clang format will change the input order which MUST be in this specific
// order
// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>
#include <chrono>
#include "camera.h"

class Window {
    public:
    struct WindowConfig {
        int width = 800;
        int height = 600;
        std::string name = "renderer2";
    };

    // Initializes the GLFW window object. With parameters from the WindowConfig
    // struct. If initialization fails throw std::runtime_error
    Window(const WindowConfig& config);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool ShouldClose() const { return glfwWindowShouldClose(window_m); }
    int GetWidth() const;
    int GetHeight() const;
    void PollEvents() const { glfwPollEvents(); }
    void SwapBuffers() const { glfwSwapBuffers(window_m); }
    void ProcessInput(Camera& camera);

    private:
    // Called automatically by GLFW when the window is resized.
    // Updates the OpenGL viewport to match the new window dimensions.
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window_m;
    static inline int instance_count = 0;

    // Time tracking for delta calculation
    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();

    // Mouse tracking for delta calculation
    double last_mouse_x = 400.0;
    double last_mouse_y = 300.0;
};
