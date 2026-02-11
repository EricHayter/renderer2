#include "mesh.h"

// clang-format off
#include "glad/gl.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <cassert>
#include <format>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture*> textures, MaterialProps material)
    : vertices(std::move(vertices)),
      indices(std::move(indices)),
      textures(std::move(textures)),
      material(material) {
    // Setup OpenGL buffers
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    VAO = vao;
    VBO = vbo;
    EBO = ebo;

    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                 &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 this->indices.size() * sizeof(unsigned int), &this->indices[0],
                 GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texture_coordinates));
    // vertex tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, tangent));
    // vertex bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (VAO) glDeleteVertexArrays(1, &*VAO);
    if (VBO) glDeleteBuffers(1, &*VBO);
    if (EBO) glDeleteBuffers(1, &*EBO);
}

Mesh::Mesh(Mesh&& other)
    : vertices(std::move(other.vertices)),
      indices(std::move(other.indices)),
      textures(std::move(other.textures)),
      VAO(other.VAO),
      VBO(other.VBO),
      EBO(other.EBO),
      material(other.material) {
    // Invalidate source to prevent double-delete
    other.VAO = std::nullopt;
    other.VBO = std::nullopt;
    other.EBO = std::nullopt;
}

Mesh& Mesh::operator=(Mesh&& other) {
    if (this != &other) {
        // Delete OUR existing resources to prevent leak
        if (VAO) glDeleteVertexArrays(1, &*VAO);
        if (VBO) glDeleteBuffers(1, &*VBO);
        if (EBO) glDeleteBuffers(1, &*EBO);

        // Transfer ownership from other
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        textures = std::move(other.textures);
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        material = other.material;

        // Invalidate source to prevent double-delete
        other.VAO = std::nullopt;
        other.VBO = std::nullopt;
        other.EBO = std::nullopt;
    }
    return *this;
}

void Mesh::Draw(Shader& shader) {
    assert(VAO && VBO && EBO && "Attempting to draw with invalid Mesh");
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    bool hasNormalMap = false;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 +
                        i);  // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        Texture::Type texture_type = textures[i]->GetType();
        std::string texture_type_str;
        switch (texture_type) {
            case Texture::Type::DIFFUSE: {
                number = std::to_string(diffuseNr++);
                texture_type_str = "Diffuse";
                break;
            }
            case Texture::Type::SPECULAR: {
                number = std::to_string(specularNr++);
                texture_type_str = "Specular";
                break;
            }
            case Texture::Type::NORMAL: {
                texture_type_str = "NormalMap";
                hasNormalMap = true;
                break;
            }
        }

        shader.SetInt(std::format("u{}{}", texture_type_str, number), {(int)i});
        textures[i]->Use();
    }
    glActiveTexture(GL_TEXTURE0);

    // Set whether to use normal mapping
    shader.SetInt("uUseNormalMap", {hasNormalMap ? 1 : 0});

    /* setting uniform values of material props */
    shader.SetFloat("uMaterial.shininess", {material.shininess});
    shader.SetFloat(
        "uMaterial.ambient",
        {material.ambient.r, material.ambient.g, material.ambient.b});
    shader.SetFloat(
        "uMaterial.diffuse",
        {material.diffuse.r, material.diffuse.g, material.diffuse.b});
    shader.SetFloat(
        "uMaterial.specular",
        {material.specular.r, material.specular.g, material.specular.b});

    // draw mesh
    glBindVertexArray(*VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
