#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <vector>

#include "camera.h"
#include "glm/vec4.hpp"
#include "model.h"
#include "shader.h"
#include "window.h"
#include "light.h"
#include "skybox.h"

class Scene {
public:
    Scene() = default;
    void Draw(Shader& shader, const Window& window);

    Camera camera;
    std::vector<std::unique_ptr<Model>> model;

    std::unique_ptr<Skybox> skybox;
    std::optional<Shader> skybox_shader;
    Light light;
};
