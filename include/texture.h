#pragma once

// clang-format off
#include "glad/gl.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <filesystem>
#include <optional>
#include <string_view>
#include "assimp/texture.h"

struct ImageData {
    ImageData(const std::filesystem::path& path);
    ImageData(const unsigned char* loaded_data, std::size_t data_length);
    ~ImageData();

    ImageData(const ImageData& other) = delete;
    ImageData& operator=(const ImageData& other) = delete;
    ImageData(ImageData&& other);
    ImageData& operator=(ImageData&& other);

    int height = 0;
    int width = 0;
    int nrchannels = 0;
    unsigned char* data = nullptr;
};

class Texture {
    public:
    enum class Type {
        DIFFUSE,
        SPECULAR,
        NORMAL,
    };

    struct Configuration {
      GLint texture_wrap_s = GL_REPEAT;
      GLint texture_wrap_t = GL_REPEAT;
      GLint texture_min_filter = GL_LINEAR_MIPMAP_LINEAR;
      GLint texture_mag_filter = GL_LINEAR;
      Type texture_type = Type::DIFFUSE;
    };

    Texture(const std::filesystem::path& path, std::string_view name, const Configuration& config);
    Texture(const aiTexture* texture, std::string_view name, const Configuration& config);
    Texture() = default;
    ~Texture();

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;
    Texture(Texture&& other);
    Texture& operator=(Texture&& other);

    void Use() const;
    bool IsExternalTexure() const { return path_m.has_value(); }
    std::filesystem::path GetPath() const { return *path_m; };
    Type GetType() const { return type_m; };
    std::optional<unsigned int> GetId() const { return texture_id_m; };

    private:
    std::optional<unsigned int> texture_id_m;
    std::string name_m;
    std::optional<std::filesystem::path> path_m;
    Type type_m;
};
