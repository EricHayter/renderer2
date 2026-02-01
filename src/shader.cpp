#include "shader.h"

#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "glad/gl.h"

bool CheckShaderCompileSuccess(unsigned int shader_id) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << "\n";
    }
    return success;
}

Shader::Shader(const std::filesystem::path& vertex_path,
               const std::filesystem::path& fragment_path) {
    if (!std::filesystem::exists(vertex_path)) {
        throw std::filesystem::filesystem_error(
            "Vertex shader file does not exist", vertex_path,
            std::make_error_code(std::errc::no_such_file_or_directory));
    }

    if (!std::filesystem::exists(fragment_path)) {
        throw std::filesystem::filesystem_error(
            "Fragment shader file does not exist", fragment_path,
            std::make_error_code(std::errc::no_such_file_or_directory));
    }

    std::ifstream vertex_stream(vertex_path);
    std::ifstream fragment_stream(fragment_path);

    std::stringstream vertex_code_stream, fragment_code_stream;

    vertex_code_stream << vertex_stream.rdbuf();
    std::string vertex_code_str = vertex_code_stream.str();
    const char* vertex_code_c_str = vertex_code_str.c_str();
    fragment_code_stream << fragment_stream.rdbuf();
    std::string fragment_code_str = fragment_code_stream.str();
    const char* fragment_code_c_str = fragment_code_str.c_str();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_code_c_str, nullptr);
    glCompileShader(vertex_shader);
    if (!CheckShaderCompileSuccess(vertex_shader)) {
        throw std::runtime_error("Failed to compile vertex shader\n");
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_code_c_str, nullptr);
    glCompileShader(fragment_shader);
    if (!CheckShaderCompileSuccess(fragment_shader)) {
        throw std::runtime_error("Failed to compile fragment shader\n");
    }

    program_id_m = glCreateProgram();
    glUseProgram(program_id_m);
    glAttachShader(program_id_m, vertex_shader);
    glAttachShader(program_id_m, fragment_shader);
    glLinkProgram(program_id_m);
}

void Shader::SetInt(std::string_view name,
                    std::initializer_list<int> values) const {
    if (values.size() == 0 || values.size() > 4) {
        throw std::runtime_error(
            "Invalid number of int values for uniform. Expected 1-4, got " +
            std::to_string(values.size()));
    }

    GLint location = glGetUniformLocation(program_id_m, name.data());
    const int* data = values.begin();

    switch (values.size()) {
        case 1:
            glUniform1i(location, data[0]);
            break;
        case 2:
            glUniform2i(location, data[0], data[1]);
            break;
        case 3:
            glUniform3i(location, data[0], data[1], data[2]);
            break;
        case 4:
            glUniform4i(location, data[0], data[1], data[2], data[3]);
            break;
    }
}

void Shader::SetBool(std::string_view name,
                     std::initializer_list<bool> values) const {
    if (values.size() == 0 || values.size() > 4) {
        throw std::runtime_error(
            "Invalid number of bool values for uniform. Expected 1-4, got " +
            std::to_string(values.size()));
    }

    GLint location = glGetUniformLocation(program_id_m, name.data());
    const bool* data = values.begin();

    switch (values.size()) {
        case 1:
            glUniform1i(location, static_cast<int>(data[0]));
            break;
        case 2:
            glUniform2i(location, static_cast<int>(data[0]),
                        static_cast<int>(data[1]));
            break;
        case 3:
            glUniform3i(location, static_cast<int>(data[0]),
                        static_cast<int>(data[1]), static_cast<int>(data[2]));
            break;
        case 4:
            glUniform4i(location, static_cast<int>(data[0]),
                        static_cast<int>(data[1]), static_cast<int>(data[2]),
                        static_cast<int>(data[3]));
            break;
    }
}

void Shader::SetFloat(std::string_view name,
                      std::initializer_list<float> values) const {
    if (values.size() == 0 || values.size() > 4) {
        throw std::runtime_error(
            "Invalid number of float values for uniform. Expected 1-4, got " +
            std::to_string(values.size()));
    }

    GLint location = glGetUniformLocation(program_id_m, name.data());
    const float* data = values.begin();

    switch (values.size()) {
        case 1:
            glUniform1f(location, data[0]);
            break;
        case 2:
            glUniform2f(location, data[0], data[1]);
            break;
        case 3:
            glUniform3f(location, data[0], data[1], data[2]);
            break;
        case 4:
            glUniform4f(location, data[0], data[1], data[2], data[3]);
            break;
    }
}
