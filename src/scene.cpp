#include "scene.h"

Scene::Scene(const std::filesystem::path& path) : model_m(path), camera_m{} {}

void Scene::Draw(Shader& shader, const Window& window) {
    glClearColor(background_color_m.r, background_color_m.g,
                 background_color_m.b, background_color_m.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();

    glm::mat4 view_mat = camera_m.GetViewMatrix();
    shader.SetMatrix4("uView", view_mat);

    glm::vec4 transformed_light_pos =
        view_mat * glm::vec4(light_pos_m.x, light_pos_m.y, light_pos_m.z, 1.0f);
    shader.SetFloat("uLightPosition",
                    {transformed_light_pos.x, transformed_light_pos.y,
                     transformed_light_pos.z});
    shader.SetFloat("uLightColor",
                    {light_color_m.r, light_color_m.g, light_color_m.b});

    // TODO in the future this is going to be a member of the model objects I
    // have
    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 normal_mat = glm::transpose(glm::inverse(view_mat * model_mat));
    shader.SetMatrix4("uNormalMatrix", normal_mat);

    shader.SetMatrix4("uModel", model_mat);

    auto [width, height] = window.GetDimensions();
    float aspect_ratio = width / (float)height;
    glm::mat4 projection_mat =
        glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
    shader.SetMatrix4("uProjection", projection_mat);

    model_m.Draw(shader);
}
