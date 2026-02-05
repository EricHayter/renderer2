#version 330 core

in vec3 FragPos;
in vec3 VertexNormal;
in vec2 TextureCoords;

uniform vec3 lightPosition;
uniform sampler2D diffuse1;
uniform sampler2D specular1;

out vec4 FragColor;

// Calculations done in view space
void main() {
    // light_dir is going away from the fragment to the light
    vec3 light_dir = normalize(lightPosition - FragPos);
    // keep lightcolor as white for now. Might be good to set the color in a uniform
    vec4 light_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Diffusion math
    vec4 diffuse_color = texture(diffuse1, TextureCoords);
    float intensity = max(dot(light_dir, normalize(VertexNormal)), 0.0f);
    vec4 diffuse =  diffuse_color * (light_color * intensity);

    FragColor = diffuse;
}
