#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "camera.h"
#include "glm/vec4.hpp"
#include "model.h"
#include "shader.h"
#include "window.h"
#include "light.h"

class Scene {
public:
    Scene() = default;
    void Draw(Shader& shader, const Window& window);

    Camera camera;
    std::vector<std::unique_ptr<Model>> model;
    Light light;
    glm::vec4 background_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};
