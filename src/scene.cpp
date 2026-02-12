#include "scene.h"

void Scene::Draw(Shader& shader, const Window& window) {
    // CLEAR SCREEN
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // GET TRANSFORMATION MATRICIES
    glm::mat4 view_mat = camera.GetViewMatrix();

    auto [width, height] = window.GetDimensions();
    float aspect_ratio = width / (float)height;

    glm::mat4 projection_mat =
        glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);

    // DISPLAY SKYBOX
    if (skybox && skybox_shader)
        skybox->Draw(*skybox_shader, view_mat, projection_mat);

    // PREPARE TO DRAW MODELS
    shader.Use();
    shader.SetMatrix4("uView", view_mat);
    shader.SetMatrix4("uProjection", projection_mat);

    auto& light_pos = light.position;
    glm::vec4 transformed_light_pos =
        view_mat * glm::vec4(light_pos.x, light_pos.y, light_pos.z, 1.0f);
    shader.SetFloat("uLight.position",
                    {transformed_light_pos.x, transformed_light_pos.y,
                     transformed_light_pos.z});

    auto& ambient = light.ambient;
    shader.SetFloat("uLight.ambient", {ambient.r, ambient.g, ambient.b});
    auto& diffuse = light.diffuse;
    shader.SetFloat("uLight.diffuse", {diffuse.r, diffuse.g, diffuse.b});
    auto& specular = light.specular;
    shader.SetFloat("uLight.specular", {specular.r, specular.g, specular.b});

    // Draw all models
    for (const auto& model_ptr : model) {
        glm::mat4 model_mat = model_ptr->GetModelMatrix();
        glm::mat4 normal_mat =
            glm::transpose(glm::inverse(view_mat * model_mat));
        shader.SetMatrix4("uNormalMatrix", normal_mat);
        shader.SetMatrix4("uModel", model_mat);

        model_ptr->Draw(shader);
    }
}
