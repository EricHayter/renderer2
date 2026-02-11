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
in vec3 vTangentLightPos;
in vec3 vTangentFragPos;

uniform Material uMaterial;
uniform Light uLight;
uniform sampler2D uDiffuse1;
uniform sampler2D uSpecular1;
uniform sampler2D uNormalMap;
uniform bool uUseNormalMap;

out vec4 FragColor;

// Calculations done in view space (or tangent space when using normal maps)
void main() {
    vec3 lightDir, viewDir, normal;

    if (uUseNormalMap) {
        // Sample normal from normal map and transform to [-1, 1]
        normal = texture(uNormalMap, vTexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);

        // Already in tangent space
        lightDir = normalize(vTangentLightPos - vTangentFragPos);
        viewDir = normalize(-vTangentFragPos);  // View is at origin
    } else {
        // Use interpolated vertex normal (view space)
        normal = normalize(vNormal);
        lightDir = normalize(uLight.position - vFragPos);
        viewDir = normalize(-vFragPos);
    }

    // Diffusion math
    vec3 diffuseColor = vec3(texture(uDiffuse1, vTexCoords));
    vec3 ambient = diffuseColor * uLight.ambient * uMaterial.ambient;

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
