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

        glm::mat4 GetModelMatrix() const;
        int GetID() const { return id_m; }

        // Transformation accessors
        glm::vec3& GetTranslation() { return translation_m; }
        glm::vec3& GetScale() { return scale_m; }
        bool& GetIsYUp() { return is_y_up_m; }

    private:
        // model data
        int id_m;
        static int next_id_s;

        glm::vec3 translation_m{0.0f, 0.0f, 0.0f};
        glm::vec3 scale_m{1.0f, 1.0f, 1.0f};
        bool is_y_up_m = true;

        std::vector<Mesh> meshes;
        std::filesystem::path directory_m;
        std::vector<std::unique_ptr<Texture>> textures_loaded;

        // Keep importer alive - scene_m points into its memory
        Assimp::Importer importer_m;
        const aiScene* scene_m;

        void loadModel(const std::filesystem::path& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type);
};
