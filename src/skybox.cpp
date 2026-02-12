#include "skybox.h"

#include <stdexcept>
#include <vector>

#include "glad/gl.h"
#include "image_data.h"

// Define static members
std::optional<unsigned int> Skybox::VAO = std::nullopt;
std::optional<unsigned int> Skybox::VBO = std::nullopt;

Skybox::Skybox(const std::filesystem::path& cubemap_path) {
    if (!VAO || !VBO) InitVAO();

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    // Load the cross-layout cubemap image
    ImageData data = ImageData(cubemap_path);

    // Determine format based on channel count
    GLenum format;
    switch (data.nrchannels) {
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
            throw std::runtime_error(
                "Unsupported channel count for skybox texture");
    }

    // Calculate face size (assuming horizontal cross layout: 4:3 aspect ratio)
    int face_width = data.width / 4;
    int face_height = data.height / 3;

    if (face_width != face_height) {
        throw std::runtime_error("Cubemap faces must be square");
    }

    int face_size = face_width;
    int channels = data.nrchannels;

    // Extract and upload each face from the cross layout
    // IMPORTANT: This assumes horizontal cross layout (4:3 aspect ratio):
    //       [py]
    // [nx] [pz] [px] [nz]
    //       [ny]

    struct FaceInfo {
        int target;
        int x_offset;
        int y_offset;
        const char* name;
    };

    FaceInfo faces[] = {
        {GL_TEXTURE_CUBE_MAP_POSITIVE_X, 2, 1, "px"},  // right
        {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 1, "nx"},  // left
        {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 1, 0, "py"},  // top
        {GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 1, 2, "ny"},  // bottom
        {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 1, 1, "pz"},  // front
        {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 3, 1, "nz"}   // back
    };

    for (const auto& face : faces) {
        // Allocate buffer for this face
        std::vector<unsigned char> face_data(face_size * face_size * channels);

        // Extract face from the cross layout
        for (int y = 0; y < face_size; y++) {
            for (int x = 0; x < face_size; x++) {
                int src_x = face.x_offset * face_size + x;
                int src_y = face.y_offset * face_size + y;
                int src_idx = (src_y * data.width + src_x) * channels;
                int dst_idx = (y * face_size + x) * channels;

                for (int c = 0; c < channels; c++) {
                    face_data[dst_idx + c] = data.data[src_idx + c];
                }
            }
        }

        glTexImage2D(face.target, 0, format, face_size, face_size, 0, format,
                     GL_UNSIGNED_BYTE, face_data.data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::Draw(Shader& shader, const glm::mat4& view_matrix,
                  const glm::mat4& projection_matrix) {
    glDepthFunc(GL_LEQUAL);  // Change depth function so skybox renders behind
                             // everything
    shader.Use();

    shader.SetMatrix4("uView", view_matrix);
    shader.SetMatrix4("uProjection", projection_matrix);

    glBindVertexArray(*VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    shader.SetInt("uSkybox", {0});

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);  // Restore default depth function
}

void Skybox::InitVAO() {
    constexpr float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

        1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

    unsigned int vao_temp, vbo_temp;
    glCreateVertexArrays(1, &vao_temp);
    VAO = vao_temp;
    glCreateBuffers(1, &vbo_temp);
    VBO = vbo_temp;
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
}
