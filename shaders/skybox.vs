#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    vTexCoords = aPos;
    // Remove translation from view matrix to keep skybox centered on camera
    mat4 view = mat4(mat3(uView));
    vec4 pos = uProjection * view * vec4(aPos, 1.0);
    // Set z = w so that after perspective divide, z/w = 1.0 (max depth)
    gl_Position = pos.xyww;
}
