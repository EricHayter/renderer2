#pragma once

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <filesystem>

struct ImageData {
    ImageData(const std::filesystem::path& path);
    ~ImageData();
    int height = 0;
    int width = 0;
    int nrchannels = 0;
    unsigned char* data = nullptr;
};

class Texture {
    public:
    struct Configuration {
      GLint texture_wrap_s = GL_REPEAT;
      GLint texture_wrap_t = GL_REPEAT;
      GLint texture_min_filter = GL_LINEAR_MIPMAP_LINEAR;
      GLint texture_mag_filter = GL_LINEAR;
    };

    Texture(const std::filesystem::path& path, const Configuration& config);
    ~Texture() { glDeleteTextures(1, &texture_id_m); }

    // Delete copy semantics
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void Use() const { glBindTexture(GL_TEXTURE_2D, texture_id_m); };

    private:
    unsigned int texture_id_m;
};
