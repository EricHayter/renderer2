#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cassert>
#include <format>
#include <stdexcept>

ImageData::ImageData(const std::filesystem::path& path) {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrchannels, 0);
    if (!data) {
        throw std::runtime_error(
            std::format("Failed to load texture data from {}\n", path.c_str()));
    }
}

ImageData::~ImageData() {
    if (data) {
        stbi_image_free(data);
    }
}

ImageData::ImageData(ImageData&& other)
    : data{other.data},
      height{other.height},
      width{other.width},
      nrchannels{other.nrchannels} {
    other.height = 0;
    other.width = 0;
    other.nrchannels = 0;
    other.data = nullptr;
}

ImageData& ImageData::operator=(ImageData&& other) {
    if (&other != this) {
        // Free existing memory to prevent leak
        if (data) {
            stbi_image_free(data);
        }

        height = other.height;
        width = other.width;
        nrchannels = other.nrchannels;
        data = other.data;

        other.data = nullptr;
    }
    return *this;
}

Texture::Texture(const std::filesystem::path& path, const Configuration& config)
    : path{path}, type{config.texture_type} {
    ImageData image_data = ImageData(path);

    // stb loads in the data with NO padding OpenGL expects that it can read in
    // the data 4 bytes at a time by default.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    texture_id_m = texture_id;
    glBindTexture(GL_TEXTURE_2D, *texture_id_m);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.texture_wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.texture_wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    config.texture_min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    config.texture_mag_filter);

    GLenum format;
    switch (image_data.nrchannels) {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            throw std::runtime_error("Unsupported channel count: " +
                                     std::to_string(image_data.nrchannels));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, image_data.width, image_data.height,
                 0, format, GL_UNSIGNED_BYTE, image_data.data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
    if (texture_id_m) glDeleteTextures(1, &*texture_id_m);
}

Texture::Texture(Texture&& other)
    : texture_id_m{other.texture_id_m},
      path{std::move(other.path)},
      type{other.type} {
    other.texture_id_m = std::nullopt;
}

Texture& Texture::operator=(Texture&& other) {
    if (&other != this) {
        // Delete existing texture to prevent leak
        if (texture_id_m) {
            glDeleteTextures(1, &*texture_id_m);
        }

        texture_id_m = other.texture_id_m;
        other.texture_id_m = std::nullopt;
        path = std::move(other.path);
        type = other.type;
    }
    return *this;
}

void Texture::Use() const {
    assert(texture_id_m && "Calling Use() on invalid Texture object\n");
    glBindTexture(GL_TEXTURE_2D, *texture_id_m);
};
