#pragma once

#include "shader.h"
#include "mesh.h"
#include <filesystem>
#include <vector>
#include "assimp/scene.h"
#include "texture.h"

class Model
{
    public:
        Model(const std::filesystem::path& path);
        void Draw(Shader &shader);

        // Stats
        size_t GetVertexCount() const;
        size_t GetTriangleCount() const;
        size_t GetMeshCount() const { return meshes.size(); }

    private:
        // model data
        std::vector<Mesh> meshes;
        std::filesystem::path directory;
        std::vector<std::unique_ptr<Texture>> textures_loaded;

        void loadModel(const std::filesystem::path& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type);
};
