// clang format will change the input order which MUST be in this specific
// order
// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>

#include "shader.h"
#include "texture.h"
#include "window_management.h"

// clang-format off
// 9 values in total (3 floats per point to make a triangle)
constexpr float TRIANGLE_DATA[] = {
    -0.5f, -0.5f,  0.0f, // bottom left
     0.5f, -0.5f,  0.0f, // bottom right
     0.0f,  0.5f,  0.0f, // top-center
};

// 9 values in total (3 floats per point to make a triangle)
constexpr float COLORED_TRIANGLE_DATA[] = {
    // coordinates         // color
    -0.25f, -0.25f,  0.0f, 1.0f, 0.0f, 0.0f,
     0.75f, -0.25f,  0.0f, 0.0f, 1.0f, 0.0f,
     0.25f,  0.75f,  0.0f, 0.0f, 0.0f, 1.0f,
};

constexpr float APHEX_TWIN_SQUARE_DATA[] = {
    // coordintes        // texture coordinates
     0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top right
     0.0f,  0.0f,  0.0f, 1.0f, 0.0f, // bottom right
    -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom left
    -1.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top left
};

constexpr unsigned int APHEX_TWIN_SQUARE_INDEX[] = {
    2, 3, 0,
    0, 1, 2,
};

// clang-format on

unsigned int CreateTriangleVAO();
unsigned int CreateColoredTriangleVAO();

unsigned int CreateTriangleVAO() {
    unsigned int VA0;
    glGenVertexArrays(1, &VA0);
    glBindVertexArray(VA0);

    // Create the buffer object
    unsigned int VB0;
    glGenBuffers(1, &VB0);

    // bind it to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, VB0);

    // Save the data to the binded target (AB0)
    glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLE_DATA), TRIANGLE_DATA,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
                          (void*)0);
    glEnableVertexAttribArray(0);

    return VA0;
}

unsigned int CreateColoredTriangleVAO() {
    unsigned int VA0;
    glGenVertexArrays(1, &VA0);
    glBindVertexArray(VA0);

    unsigned int VB0;
    glCreateBuffers(1, &VB0);
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(COLORED_TRIANGLE_DATA),
                 COLORED_TRIANGLE_DATA, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0x00);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(
        1);  // MAKE SURE TO ENABLE FOR ATTRIBUTE AT INDEX 1!!

    return VA0;
}

unsigned int CreateAphexTwinSquareVAO() {
    unsigned int VA0;
    glGenVertexArrays(1, &VA0);
    glBindVertexArray(VA0);

    unsigned int VB0;
    glGenBuffers(1, &VB0);
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(APHEX_TWIN_SQUARE_DATA),
                 APHEX_TWIN_SQUARE_DATA, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)0x00);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EB0;
    glGenBuffers(1, &EB0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(APHEX_TWIN_SQUARE_INDEX),
                 APHEX_TWIN_SQUARE_INDEX, GL_STATIC_DRAW);

    return VA0;
}

int main() {
    auto window_opt = window::InitWindow({});
    if (!window_opt.has_value()) return -1;
    GLFWwindow* window = *window_opt;

    // Use glad to import everything for OpenGL
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader uniform_shader =
        Shader("../shaders/uniform.vs", "../shaders/uniform.fs");
    Shader gradient_shader =
        Shader("../shaders/gradient.vs", "../shaders/gradient.fs");
    Shader texture_shader =
        Shader("../shaders/texture.vs", "../shaders/texture.fs");

    // create the VAO
    unsigned int VA0 = CreateTriangleVAO();

    // create the VAO
    unsigned int VA1 = CreateColoredTriangleVAO();

    Texture aphex_twin_texture = Texture("../textures/aphex_twin.jpg", {});
    unsigned int VA2 = CreateAphexTwinSquareVAO();

    float colors[] = {0.0f, 0.0f, 0.0f, 1.0f};
    int selected_color = 0;
    float increase_amount = 0.01f;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // handling input
        window::ProcessInput(window);

        // Rendering
        glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // update our shader using the uniforms
        if (colors[selected_color] >= 1.0f) {
            colors[selected_color] = 0.0f;
            selected_color = (selected_color + 1) % 3;
        }
        colors[selected_color] += increase_amount;

        uniform_shader.Use();
        uniform_shader.SetFloat("ourColor",
                                {colors[0], colors[1], colors[2], colors[3]});
        glBindVertexArray(VA0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        gradient_shader.Use();
        glBindVertexArray(VA1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        texture_shader.Use();
        texture_shader.SetInt("Texture", {0});
        aphex_twin_texture.Use();
        glBindVertexArray(VA2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap buffers and poll for IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
