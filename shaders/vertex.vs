#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uNormalMatrix;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoords;

void main() {
    vFragPos = vec3(uView * uModel * vec4(aPos, 1.0f));
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
    vNormal = vec3(uNormalMatrix * vec4(aNormal, 1.0f));
    vTexCoords = aTexCoords;
}
