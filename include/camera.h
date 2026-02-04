#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct Camera {
    enum class MoveDirection {
        FORWARD,
        BACKWARD,
        RIGHT,
        LEFT,
    };

    glm::mat4 GetViewMatrix();
    void UpdatePosition(MoveDirection direction, float delta_ms);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -1.0f);

    // noramlized vector for where the camera is currently pointing
    glm::vec3 pointing_at = glm::vec3(0.0f, 0.0f, -1.0f);
    float fov = glm::radians(45.0f);

    // units / ms of the camera. Used in the UpdatePosition function.
    float camera_velocity = 4.0f / 1000;
};
