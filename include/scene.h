#pragma once

#include <filesystem>

#include "camera.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "model.h"
#include "shader.h"
#include "window.h"
#include "light.h"

class Scene {
    public:
    Scene(const std::filesystem::path& path);
    void Draw(Shader& shader, const Window& window);
    Camera& GetCamera() { return camera_m; }

    // Lighting accessors
    Light& GetLight() { return light_m; }

    // Model accessor
    Model& GetModel() { return model_m; }

    private:
    Camera camera_m;
    Model model_m;
    Light light_m;

    // background color of the scene
    glm::vec4 background_color_m = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};
