#include "image_data.h"

#define STB_IMAGE_IMPLEMENTATION
#include <format>
#include <stdexcept>

#include "stb/stb_image.h"

ImageData::ImageData(const std::filesystem::path& path) {
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
