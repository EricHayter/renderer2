#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoords;

uniform Material uMaterial;
uniform Light uLight;
uniform sampler2D uDiffuse1;
uniform sampler2D uSpecular1;

out vec4 FragColor;

// Calculations done in view space
void main() {
    // lightDir is going away from the fragment to the light
    vec3 lightDir = normalize(uLight.position - vFragPos);

    // coming from point to my screen
    vec3 viewDir = normalize(-vFragPos);

    // Diffusion math
    vec3 diffuseColor = vec3(texture(uDiffuse1, vTexCoords));
    vec3 ambient = diffuseColor * uLight.ambient * uMaterial.ambient;

    vec3 normal = normalize(vNormal);
    float intensity = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diffuseColor * intensity * uLight.diffuse * uMaterial.diffuse;

    // Specular math
    vec3 specularColor = vec3(texture(uSpecular1, vTexCoords));
    vec3 reflection = reflect(-lightDir, normal);
    intensity = pow(max(dot(viewDir, reflection), 0.0), uMaterial.shininess);
    vec3 specular = specularColor * intensity * uLight.specular * uMaterial.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}
