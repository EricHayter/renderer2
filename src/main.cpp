#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>

constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;

constexpr float triangleData[] = {
    -0.5f,  -0.5f,  0.0f,
     0.5f,  -0.5f,  0.0f,
     0.0f,   0.5f,  0.0f,
};

constexpr const char* const VERTEX_SHADER_SOURCE = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

constexpr const char* const FRAGMENT_SHADER_SOURCE = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(10.f, 0.f, 0.f, 1.0f);\n"
    "}\0";


unsigned int CreateTriangleVAO();
std::optional<unsigned int> CompileShaderProgram();
void FramebufferSizeCallbank(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow *window);
bool CheckShaderCompileSuccess(unsigned int shader);

unsigned int CreateTriangleVAO() {
    unsigned int AV0;
    glGenVertexArrays(1, &AV0);
    glBindVertexArray(AV0);

    // Create the buffer object
    unsigned int AB0;
    glGenBuffers(1, &AB0);

    // bind it to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, AB0);

    // Save the data to the binded target (AB0)
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    return AV0;
}

std::optional<unsigned int> CompileShaderProgram() {
    unsigned vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &VERTEX_SHADER_SOURCE, nullptr);
    glCompileShader(vert_shader);
    if (!CheckShaderCompileSuccess(vert_shader))
        return {};

    unsigned frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &FRAGMENT_SHADER_SOURCE, nullptr);
    glCompileShader(frag_shader);
    if (!CheckShaderCompileSuccess(frag_shader))
        return {};

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    return shader_program;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool CheckShaderCompileSuccess(unsigned int shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
    }
    return success;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
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
    if (!shader_program)
        return -1;

    // create the VAO
    unsigned int VA0 = CreateTriangleVAO();

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // handling input
        ProcessInput(window);

        // Rendering
        glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(*shader_program);
        glBindVertexArray(VA0);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // swap buffers and poll for IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

