#include "camera.h"

#include <algorithm>

#include "glm/geometric.hpp"

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + pointing_at,
                       glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdatePosition(Camera::MoveDirection direction, float delta_ms) {
    float movement = delta_ms * camera_velocity;

    switch (direction) {
        case MoveDirection::FORWARD: {
            position += pointing_at * movement;
            break;
        }
        case MoveDirection::BACKWARD: {
            position -= pointing_at * movement;
            break;
        }
        case MoveDirection::RIGHT: {
            glm::vec3 right = glm::normalize(
                glm::cross(pointing_at, glm::vec3(0.0f, 1.0f, 0.0f)));
            position += right * movement;
            break;
        }
        case MoveDirection::LEFT: {
            glm::vec3 right = glm::normalize(
                glm::cross(pointing_at, glm::vec3(0.0f, 1.0f, 0.0f)));
            position -= right * movement;
            break;
        }
        case MoveDirection::UP: {
            position += glm::vec3(0.0f, 1.0f, 0.0f) * movement;
            break;
        }
        case MoveDirection::DOWN: {
            position -= glm::vec3(0.0f, 1.0f, 0.0f) * movement;
            break;
        }
    }
}

void Camera::UpdatePointingDirection(float delta_x, float delta_y) {
    // Update angles based on mouse movement
    yaw += delta_x * camera_sensitivity;
    pitch -= delta_y * camera_sensitivity;  // Invert Y: screen Y down = look up

    // Clamp pitch to prevent camera flipping
    pitch = std::clamp(pitch, -89.0f, 89.0f);

    // Convert spherical coordinates (yaw, pitch) to direction vector
    pointing_at = glm::normalize(
        glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                  sin(glm::radians(pitch)),
                  sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}
