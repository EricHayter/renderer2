#version 330 core
in vec2 TextureCoords;
uniform sampler2D diffuse1;

out vec4 FragColor;
void main() {
    FragColor = texture(diffuse1, TextureCoords);
}
