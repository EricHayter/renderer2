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
#include "window_management.h"

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

    auto window_opt = window::InitWindow({});
    if (!window_opt.has_value()) return -1;
    GLFWwindow* window = *window_opt;

    // Use glad to import everything for OpenGL
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Model model = Model(model_path);
    Shader shader = Shader("../shaders/vertex.vs", "../shaders/fragment.fs");

    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // handling input
        window::ProcessInput(window);

        // Rendering
        glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection =
            glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
        glm::mat4 view =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
        shader.SetMatrix4("Translate", projection * view);

        model.Draw(shader);

        // swap buffers and poll for IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
