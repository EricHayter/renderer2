#include "window.h"

#include <chrono>
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

void Window::ProcessInput(Camera& camera) {
    if (glfwGetKey(window_m, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_m, true);

    // Camera Control
    // Keyboard
    using namespace std::chrono;
    auto current = steady_clock::now();
    int delta_ms = duration_cast<milliseconds>(current - last_time).count();
    last_time = current;

    if (glfwGetKey(window_m, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window_m, GLFW_KEY_UP) == GLFW_PRESS)
        camera.UpdatePosition(Camera::MoveDirection::FORWARD, delta_ms);
    if (glfwGetKey(window_m, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window_m, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.UpdatePosition(Camera::MoveDirection::LEFT, delta_ms);
    if (glfwGetKey(window_m, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window_m, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.UpdatePosition(Camera::MoveDirection::BACKWARD, delta_ms);
    if (glfwGetKey(window_m, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(window_m, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.UpdatePosition(Camera::MoveDirection::RIGHT, delta_ms);
    if (glfwGetKey(window_m, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.UpdatePosition(Camera::MoveDirection::UP, delta_ms);
    if (glfwGetKey(window_m, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.UpdatePosition(Camera::MoveDirection::DOWN, delta_ms);

    // Mouse look
    if (glfwGetMouseButton(window_m, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double current_x, current_y;
        glfwGetCursorPos(window_m, &current_x, &current_y);
        if (last_mouse_x && last_mouse_y) {
            double delta_x = current_x - *last_mouse_x;
            double delta_y = current_y - *last_mouse_y;

            last_mouse_x = current_x;
            last_mouse_y = current_y;

            camera.UpdatePointingDirection(delta_x, delta_y);
        } else {
            glfwSetInputMode(window_m, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            last_mouse_x = current_x;
            last_mouse_y = current_y;
        }
    } else if (last_mouse_x && last_mouse_y) {
        glfwSetInputMode(window_m, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        last_mouse_x = std::nullopt;
        last_mouse_y = std::nullopt;
    }
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width,
                                     int height) {
    glViewport(0, 0, width, height);
}
