#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoords;
uniform mat4 Translate;
out vec2 TextureCoords;
void main() {
    gl_Position = Translate * vec4(aPos, 1.0f);
    TextureCoords = aTextureCoords;
}
