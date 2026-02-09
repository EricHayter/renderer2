#pragma once

#include "glm/vec3.hpp"

struct Light {
    glm::vec3 position;
    glm::vec3 ambient = glm::vec3(0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f);
    glm::vec3 specular = glm::vec3(1.0f);
};
