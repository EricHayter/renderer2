#version 330 core

in vec3 FragPos;
in vec3 VertexNormal;
in vec2 TextureCoords;


uniform sampler2D diffuse1;
uniform sampler2D specular1;

out vec4 FragColor;

void main() {
    vec4 light_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec3 light_dir = vec3(0.0f, 0.0f, 1.0f);

    vec4 diffuse_color = texture(diffuse1, TextureCoords);

    float intensity = max(dot(light_dir, normalize(VertexNormal)), 0.0f);
    vec4 diffuse =  diffuse_color * (light_color * intensity);

    FragColor = diffuse;
}
