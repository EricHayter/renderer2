#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <cassert>
#include <format>
#include <stdexcept>

#include "stb/stb_image.h"

ImageData::ImageData(const std::filesystem::path& path) {
    // TODO This doesn't seem to be needed for actual texture files...
    //    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrchannels, 0);
    if (!data) {
        throw std::runtime_error(
            std::format("Failed to load texture data from {}\n", path.c_str()));
    }
}

ImageData::ImageData(const unsigned char* loaded_data,
                     std::size_t data_length) {
    data = stbi_load_from_memory(loaded_data, data_length, &width, &height,
                                 &nrchannels, 0);
    if (!data) {
        throw std::runtime_error("Unable to load texture data from memory");
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

Texture::Texture(const std::filesystem::path& path, std::string_view name,
                 const Configuration& config)
    : path_m{path}, type_m{config.texture_type}, name_m{name} {
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

Texture::Texture(const aiTexture* texture, std::string_view name,
                 const Configuration& config)
    : name_m{name}, type_m{config.texture_type} {
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

    /* data is in a compresed format we need something like stb_image.h to load
     * it */
    if (texture->mHeight == 0) {
        // TODO not sure about this cast... each pixel value is a unsigned
        // byte... I don't think this is going to work with alphas
        ImageData image_data =
            ImageData((unsigned char*)texture->pcData, texture->mWidth);
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

        glTexImage2D(GL_TEXTURE_2D, 0, format, image_data.width,
                     image_data.height, 0, format, GL_UNSIGNED_BYTE,
                     image_data.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        return;
    }

    /* Based on the following handling from the assimp docs
     * https://the-asset-importer-lib-documentation.readthedocs.io/en/latest/usage/use_the_lib.html
     *
     * uncompressed stuff is always in RGB8888 format
     */
    GLenum format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->mWidth, texture->mHeight, 0,
                 format, GL_UNSIGNED_BYTE, texture->pcData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(std::array<unsigned char, 3> rgb, std::string_view name,
                 const Configuration& config) {
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 rgb.data());
}

Texture::~Texture() {
    if (texture_id_m) glDeleteTextures(1, &*texture_id_m);
}

Texture::Texture(Texture&& other)
    : texture_id_m{other.texture_id_m},
      name_m{std::move(other.name_m)},
      path_m{std::move(other.path_m)},
      type_m{other.type_m} {
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
        name_m = std::move(other.name_m);
        path_m = std::move(other.path_m);
        type_m = other.type_m;
    }
    return *this;
}

void Texture::Use() const {
    assert(texture_id_m && "Calling Use() on invalid Texture object\n");
    glBindTexture(GL_TEXTURE_2D, *texture_id_m);
};
