#include "window.h"

#include <stdexcept>

Window::Window(const WindowConfig& config) {
    bool first_window = (instance_count == 0);

    if (first_window) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_m = glfwCreateWindow(config.width, config.height,
                                config.name.c_str(), NULL, NULL);
    if (window_m == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window_m);

    if (first_window) {
        if (!gladLoadGL(glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    glfwSetFramebufferSizeCallback(window_m, FramebufferSizeCallback);
    instance_count++;
}

Window::~Window() {
    glfwDestroyWindow(window_m);
    instance_count--;
    if (instance_count == 0) {
        glfwTerminate();
    }
}

int Window::GetWidth() const {
    int w, h;
    glfwGetFramebufferSize(window_m, &w, &h);
    return w;
}

int Window::GetHeight() const {
    int w, h;
    glfwGetFramebufferSize(window_m, &w, &h);
    return h;
}

void Window::ProcessInput() {
    if (glfwGetKey(window_m, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_m, true);
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width,
                                     int height) {
    glViewport(0, 0, width, height);
}
