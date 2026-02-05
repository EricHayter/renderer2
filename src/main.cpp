// clang format will change the input order which MUST be in this specific
// order
// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <filesystem>
#include <format>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "glm/ext/matrix_clip_space.hpp"
#include "model.h"
#include "shader.h"
#include "window.h"

int main(int argc, const char** const argv) {
    if (argc != 2) {
        std::cout << std::format("Usage: {} FILE\n", argv[0]);
        return -1;
    }
    std::filesystem::path model_path = argv[1];
    if (!std::filesystem::exists(model_path)) {
        std::cout << std::format("Could'nt find model at '{}'\n",
                                 model_path.string());
        return -1;
    }

    Window window({});

    Model model = Model(model_path);
    Shader shader = Shader("../shaders/vertex.vs", "../shaders/fragment.fs");

    glEnable(GL_DEPTH_TEST);

    Camera camera{};

    // Render loop
    while (!window.ShouldClose()) {
        // handling input
        window.ProcessInput(camera);

        // Rendering
        glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model_mat = glm::mat4(1.0f);
        shader.SetMatrix4("model", model_mat);

        glm::mat4 view_mat = camera.GetViewMatrix();
        shader.SetMatrix4("view", view_mat);

        glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 10.0f);
        light_pos =
            view_mat * glm::vec4(light_pos.x, light_pos.y, light_pos.z, 1.0f);
        shader.SetFloat("lightPosition",
                        {light_pos.x, light_pos.y, light_pos.z});

        glm::mat4 normal_mat =
            glm::transpose(glm::inverse(view_mat * model_mat));
        shader.SetMatrix4("normalMat", normal_mat);

        float aspect_ratio = window.GetWidth() / (float)window.GetHeight();
        glm::mat4 projection_mat =
            glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
        shader.SetMatrix4("projection", projection_mat);

        model.Draw(shader);

        // swap buffers and poll for IO events
        window.SwapBuffers();
        window.PollEvents();
    }
    return 0;
}
