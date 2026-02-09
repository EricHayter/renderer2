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

    Camera camera;
    Model model;
    Light light;
    glm::vec4 background_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};
