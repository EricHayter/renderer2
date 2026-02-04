#include "camera.h"

#include "glm/geometric.hpp"

glm::mat4 Camera::GetViewMatrix() {
    return glm::translate(glm::mat4(1.0f), position);
}

void Camera::UpdatePosition(Camera::MoveDirection direction, float delta_ms) {
    switch (direction) {
        case MoveDirection::FORWARD: {
            position -= pointing_at * delta_ms * camera_velocity;
            break;
        }
        case MoveDirection::BACKWARD: {
            position += pointing_at * delta_ms * camera_velocity;
            break;
        }
        case MoveDirection::RIGHT: {
            position += glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), pointing_at) *
                        delta_ms * camera_velocity;
            break;
        }
        case MoveDirection::LEFT: {
            position -= glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), pointing_at) *
                        delta_ms * camera_velocity;
            break;
        }
    }
}
