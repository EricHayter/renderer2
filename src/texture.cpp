#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <format>
#include <stdexcept>

ImageData::ImageData(const std::filesystem::path& path) {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrchannels, 0);
    if (!data) {
        throw std::runtime_error(
            std::format("Failed to texture data from {}\n", path.c_str()));
    }
}

ImageData::~ImageData() {
    if (data) {
        stbi_image_free(data);
    }
}

Texture::Texture(const std::filesystem::path& path,
                 const Configuration& config) {
    ImageData image_data = ImageData(path);

    // stb loads in the data with NO padding OpenGL expects that it can read in
    // the data 4 bytes at a time by default.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture_id_m);
    glBindTexture(GL_TEXTURE_2D, texture_id_m);

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
