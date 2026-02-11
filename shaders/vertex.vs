#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 uNormalMatrix;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uLightPos;
uniform bool uUseNormalMap;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vTangentLightPos;
out vec3 vTangentFragPos;

void main() {
    vFragPos = vec3(uView * uModel * vec4(aPos, 1.0));
    gl_Position = uProjection * vec4(vFragPos, 1.0);
    vTexCoords = aTexCoords;

    if (uUseNormalMap) {
        // Transform T, B, N to view space for consistency with vFragPos
        vec3 T = normalize(vec3(uNormalMatrix * vec4(aTangent, 0.0)));
        vec3 B = normalize(vec3(uNormalMatrix * vec4(aBitangent, 0.0)));
        vec3 N = normalize(vec3(uNormalMatrix * vec4(aNormal, 0.0)));
        mat3 TBN = transpose(mat3(T, B, N));

        // Transform to tangent space (no need for view position - it's 0 in view space)
        vTangentLightPos = TBN * uLightPos;
        vTangentFragPos = TBN * vFragPos;
    } else {
        // Regular normal for non-normal-mapped models
        vNormal = vec3(uNormalMatrix * vec4(aNormal, 0.0));
    }
}
