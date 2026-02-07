#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform sampler2D uDiffuse1;
uniform sampler2D uSpecular1;

out vec4 FragColor;

// Calculations done in view space
void main() {
    float shininess = 32.0f;
    float specularStrength = 0.5f;
    vec4 lightColor = vec4(uLightColor, 1.0f);

    // lightDir is going away from the fragment to the light
    vec3 lightDir = normalize(uLightPosition - vFragPos);

    // Diffusion math
    vec4 diffuseColor = texture(uDiffuse1, vTexCoords);
    float intensity = max(dot(lightDir, normalize(vNormal)), 0.0f);
    vec4 diffuse = diffuseColor * (lightColor * intensity);

    // Specular math
    vec4 specularColor = texture(uSpecular1, vTexCoords);
    intensity = max(dot(reflect(-lightDir, vNormal), normalize(-vFragPos)), 0.0f);
    vec4 specular = specularColor * (lightColor * pow(intensity, shininess) * specularStrength);

    FragColor = diffuse + specular;
}
