#pragma once

#include "shader.h"
#include "mesh.h"
#include <filesystem>
#include <vector>
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Model
{
    public:
        Model(const std::filesystem::path& path);
        void Draw(Shader &shader);

        // Stats
        size_t GetVertexCount() const;
        size_t GetTriangleCount() const;
        size_t GetMeshCount() const { return meshes.size(); }
        const std::vector<std::unique_ptr<Texture>>& GetTextures() const { return textures_loaded; }

        glm::mat4 GetModelMatrix() const;
        int GetID() const { return id; }

        // Transformation properties
        glm::vec3 translation{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        bool is_y_up = true;

    private:
        // model data
        int id;
        static int next_id;

        std::vector<Mesh> meshes;
        std::filesystem::path directory;
        std::vector<std::unique_ptr<Texture>> textures_loaded;

        // Default textures for meshes without diffuse/specular maps
        std::unique_ptr<Texture> default_diffuse;
        std::unique_ptr<Texture> default_specular;

        // Keep importer alive - scene points into its memory
        Assimp::Importer importer;
        const aiScene* scene;

        void loadModel(const std::filesystem::path& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type);
};
