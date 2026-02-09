#include "model.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <stdexcept>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "stb/stb_image.h"
#include "texture.h"

// Initialize static ID counter
int Model::next_id = 0;

Model::Model(const std::filesystem::path &path) : id(next_id++) {
    loadModel(path);
}

void Model::Draw(Shader &shader) {
    shader.Use();
    for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
}

size_t Model::GetVertexCount() const {
    size_t total = 0;
    for (const auto &mesh : meshes) {
        total += mesh.GetVertices().size();
    }
    return total;
}

size_t Model::GetTriangleCount() const {
    size_t total = 0;
    for (const auto &mesh : meshes) {
        total += mesh.GetIndices().size() / 3;
    }
    return total;
}

glm::mat4 Model::GetModelMatrix() const {
    glm::mat4 model_matrix = glm::mat4(1.0f);

    // Apply translation
    model_matrix = glm::translate(model_matrix, translation);

    // Apply coordinate system conversion if needed (Y-up to Z-up)
    if (!is_y_up) {
        // Rotate 90 degrees around X axis to convert Y-up to Z-up
        model_matrix = glm::rotate(model_matrix, glm::radians(90.0f),
                                   glm::vec3(1.0f, 0.0f, 0.0f));
    }

    // Apply scale
    model_matrix = glm::scale(model_matrix, scale);

    return model_matrix;
}

void Model::loadModel(const std::filesystem::path &path) {
    scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString()
                  << std::endl;
        return;
    }
    directory = path.parent_path();

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture *> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        if (mesh->mNormals) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 texture_coordinates;
            texture_coordinates.x = mesh->mTextureCoords[0][i].x;
            texture_coordinates.y = mesh->mTextureCoords[0][i].y;
            vertex.texture_coordinates = texture_coordinates;
        } else {
            vertex.texture_coordinates = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    float shininess = 32.0f;
    glm::vec3 ambient(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse(1.0f, 1.0f, 1.0f);
    glm::vec3 specular(1.0f, 1.0f, 1.0f);

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // Load textures
        std::vector<Texture *> diffuseMaps =
            loadMaterialTextures(material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture *> specularMaps =
            loadMaterialTextures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(),
                        specularMaps.end());

        // Extract material properties - using exact old working logic
        aiColor3D aiAmbient(1.0f, 1.0f, 1.0f);
        aiColor3D aiDiffuse(1.0f, 1.0f, 1.0f);
        aiColor3D aiSpecular(1.0f, 1.0f, 1.0f);

        material->Get(AI_MATKEY_SHININESS, shininess);
        material->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse);
        material->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular);

        ambient = glm::vec3(aiAmbient.r, aiAmbient.g, aiAmbient.b);
        diffuse = glm::vec3(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b);
        specular = glm::vec3(aiSpecular.r, aiSpecular.g, aiSpecular.b);
    }

    return Mesh(vertices, indices, textures, shininess, ambient, diffuse,
                specular);
}

std::vector<Texture *> Model::loadMaterialTextures(aiMaterial *mat,
                                                   aiTextureType type) {
    const std::unordered_map<aiTextureType, Texture::Type> texture_type_map{
        {aiTextureType_DIFFUSE, Texture::Type::DIFFUSE},
        {aiTextureType_SPECULAR, Texture::Type::SPECULAR}};
    if (!texture_type_map.contains(type)) {
        throw std::runtime_error(
            std::format("Unknown aiTextureType value {}", (int)type));
    }
    Texture::Type texture_type = texture_type_map.at(type);

    std::vector<Texture *> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString texture_name;
        mat->GetTexture(type, i, &texture_name);
        // Normalize path separators (handle Windows-style paths in .obj files)
        std::string texture_name_string(texture_name.C_Str());
        std::replace(texture_name_string.begin(), texture_name_string.end(),
                     '\\', '/');

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (textures_loaded[j]->IsExternalTexure() &&
                textures_loaded[j]->GetPath() == texture_name_string) {
                textures.push_back(textures_loaded[j].get());
                skip = true;
                break;
            }
        }
        if (!skip) {  // if texture hasn't been loaded already, load it
            // Texture is internal to the model file
            if (texture_name_string.starts_with("*")) {
                int index = std::stoi(texture_name_string.substr(1));
                aiTexture *embedded = scene->mTextures[index];
                auto texture = std::make_unique<Texture>(
                    embedded, texture_name_string,
                    Texture::Configuration{.texture_type = texture_type});
                textures.push_back(texture.get());
                textures_loaded.push_back(std::move(texture));
            } else {
                // texture is external to the model file (likely in textures/)
                auto texture = std::make_unique<Texture>(
                    directory / texture_name_string, texture_name_string,
                    Texture::Configuration{.texture_type = texture_type});
                textures.push_back(texture.get());
                textures_loaded.push_back(std::move(texture));
            }
        }
    }
    return textures;
}
