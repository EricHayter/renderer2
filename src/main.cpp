// clang format will change the input order which MUST be in this specific
// order
// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>
#include <optional>

constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;

// clang-format off
// 9 values in total (3 floats per point to make a triangle)
constexpr float TRIANGLE_DATA[] = {
    -0.5f, -0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
     0.0f,  0.5f,  0.0f,
};

// 9 values in total (3 floats per point to make a triangle)
constexpr float COLORED_TRIANGLE_DATA[] = {
    -0.25f, -0.25f,  0.0f, 1.0f, 0.0f, 0.0f,
     0.75f, -0.25f,  0.0f, 0.0f, 1.0f, 0.0f,
     0.25,  0.75f,  0.0f, 0.0f, 0.0f, 1.0f,
};
// clang-format on

constexpr const char* const VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

constexpr const char* const FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main() {\n"
    "   FragColor = ourColor;\n"
    "}\0";

constexpr const char* const COLORED_VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos, 1.0f);\n"
    "   ourColor = aColor;\n"
    "}\0";

constexpr const char* const COLORED_FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\0";

unsigned int CreateTriangleVAO();
std::optional<unsigned int> CompileShaderProgram();
void FramebufferSizeCallbank(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
bool CheckShaderCompileSuccess(unsigned int shader);

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

std::optional<unsigned int> CompileShaderProgram() {
    unsigned vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &VERTEX_SHADER_SOURCE, nullptr);
    glCompileShader(vert_shader);
    if (!CheckShaderCompileSuccess(vert_shader)) return {};

    unsigned frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &FRAGMENT_SHADER_SOURCE, nullptr);
    glCompileShader(frag_shader);
    if (!CheckShaderCompileSuccess(frag_shader)) return {};

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    return shader_program;
}

std::optional<unsigned int> CompileColorShaderProgram() {
    unsigned vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &COLORED_VERTEX_SHADER_SOURCE, nullptr);
    glCompileShader(vert_shader);
    if (!CheckShaderCompileSuccess(vert_shader)) return {};

    unsigned frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &COLORED_FRAGMENT_SHADER_SOURCE, nullptr);
    glCompileShader(frag_shader);
    if (!CheckShaderCompileSuccess(frag_shader)) return {};

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    return shader_program;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool CheckShaderCompileSuccess(unsigned int shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << "\n";
    }
    return success;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Use glad to import everything for OpenGL
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // create the shader
    auto shader_program = CompileShaderProgram();
    if (!shader_program) return -1;

    // create the shader
    auto colored_shader_program = CompileColorShaderProgram();
    if (!colored_shader_program) return -1;

    // create the VAO
    unsigned int VA0 = CreateTriangleVAO();

    // create the VAO
    unsigned int VA1 = CreateColoredTriangleVAO();

    float colors[] = {0.0f, 0.0f, 0.0f, 1.0f};
    int selected_color = 0;
    float increase_amount = 0.01f;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // handling input
        ProcessInput(window);

        // Rendering
        glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // update our shader using the uniforms
        if (colors[selected_color] >= 1.0f) {
            colors[selected_color] = 0.0f;
            selected_color = (selected_color + 1) % 3;
        }
        colors[selected_color] += increase_amount;

        int ourColorLocation =
            glGetUniformLocation(*shader_program, "ourColor");
        glUseProgram(*shader_program);
        glUniform4f(ourColorLocation, colors[0], colors[1], colors[2],
                    colors[3]);

        glBindVertexArray(VA0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(*colored_shader_program);
        glBindVertexArray(VA1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap buffers and poll for IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
