#pragma once

#include <filesystem>

#include "camera.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "model.h"
#include "shader.h"
#include "window.h"

class Scene {
    public:
    Scene(const std::filesystem::path& path);
    void Draw(Shader& shader, const Window& window);
    Camera& GetCamera() { return camera_m; }

    // Lighting accessors
    glm::vec3& GetLightPosition() { return light_pos_m; }
    glm::vec3& GetLightColor() { return light_color_m; }

    private:
    Camera camera_m;
    Model model_m;

    // background color of the scene
    glm::vec4 background_color_m = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 light_pos_m = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 light_color_m = glm::vec3(1.0f, 1.0f, 1.0f);
};
