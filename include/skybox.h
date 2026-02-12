#pragma once

#include <filesystem>
#include <optional>

#include "glm/glm.hpp"
#include "shader.h"

class Skybox {
    public:
    Skybox(const std::filesystem::path& cubemap_path);
    void Draw(Shader& shader, const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

    private:
    static void InitVAO();

    private:
    unsigned int texture_id;
    static std::optional<unsigned int> VAO;
    static std::optional<unsigned int> VBO;

};
