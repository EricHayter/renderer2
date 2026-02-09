#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>
#include <optional>

#include "shader.h"
#include "texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
};

class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures,
             float shininess, glm::vec3 ambient,
             glm::vec3 diffuse, glm::vec3 specular);
        ~Mesh();
        Mesh(const Mesh& other) = delete;
        Mesh& operator=(const Mesh& other) = delete;
        Mesh(Mesh&& other);
        Mesh& operator=(Mesh&& other);

        void Draw(Shader& shader);

        // Const getters for read-only access
        const std::vector<Vertex>& GetVertices() const { return vertices; }
        const std::vector<unsigned int>& GetIndices() const { return indices; }
        const std::vector<Texture*>& GetTextures() const { return textures; }

    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture*> textures;
        std::optional<unsigned int> VAO, VBO, EBO;

        float shininess_m;
        glm::vec3 ambient_m;
        glm::vec3 diffuse_m;
        glm::vec3 specular_m;
};
