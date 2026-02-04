#include "model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <format>
#include <iostream>

#include "texture.h"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

Model::Model(const std::filesystem::path &path) { loadModel(path); }

void Model::Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
}

void Model::loadModel(const std::filesystem::path &path) {
    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
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
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture *> diffuseMaps =
            loadMaterialTextures(material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture *> specularMaps =
            loadMaterialTextures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(),
                        specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
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
        aiString texture_path_str;
        mat->GetTexture(type, i, &texture_path_str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (textures_loaded[j]->GetPath() == texture_path_str.C_Str()) {
                textures.push_back(textures_loaded[j].get());
                skip = true;
                break;
            }
        }
        if (!skip) {  // if texture hasn't been loaded already, load it
            auto texture = std::make_unique<Texture>(
                directory / texture_path_str.C_Str(),
                Texture::Configuration{.texture_type = texture_type});
            textures.push_back(texture.get());
            textures_loaded.push_back(std::move(texture));
        }
    }
    return textures;
}
