#pragma once

#include <filesystem>
#include <string_view>
#include "glad/gl.h"

bool CheckShaderCompileSuccess(unsigned int shader_id);

class Shader {
    public:
    Shader(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path);

    unsigned int GetId() const { return program_id_m; }
    void Use() const { glUseProgram(program_id_m); }

    // Set uniform values
    void SetInt(std::string_view name, std::initializer_list<int> values) const;
    void SetBool(std::string_view name, std::initializer_list<bool> values) const;
    void SetFloat(std::string_view name, std::initializer_list<float> values) const;

    private:
    unsigned int program_id_m;
};
