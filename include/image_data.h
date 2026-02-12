#pragma once

#include <cstddef>
#include <filesystem>

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
