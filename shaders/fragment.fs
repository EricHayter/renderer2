#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec3 uLightPosition;
uniform sampler2D uDiffuse1;
uniform sampler2D uSpecular1;

out vec4 FragColor;

// Calculations done in view space
void main() {
    // lightDir is going away from the fragment to the light
    vec3 lightDir = normalize(uLightPosition - vFragPos);
    // keep lightColor as white for now. Might be good to set the color in a uniform
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Diffusion math
    vec4 diffuseColor = texture(uDiffuse1, vTexCoords);
    float intensity = max(dot(lightDir, normalize(vNormal)), 0.0f);
    vec4 diffuse = diffuseColor * (lightColor * intensity);

    FragColor = diffuse;
}
