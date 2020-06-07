#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const char *vertexShaderText = "#version 330\n\
layout(location = 0) in vec3 pos;\n\
\n\
void main() {\n\
  gl_Position = vec4(pos, 1.0);\n\
}";

static const char *fragmentShaderText = "#version 330\n\
layout(location = 0) out vec4 color;\n\
\n\
void main() {\n\
  color = vec4(1.0, 0.0, 0.0, 1.0);\n\
}";

void CreateTriangle(GLuint &VAO, GLuint &VBO)
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar *theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result;
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLint logLength;
        glGetShaderiv(theShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> eLog(logLength);
        glGetShaderInfoLog(theShader, logLength, nullptr, eLog.data());

        std::cerr << __FUNCTION__ << ":" << __LINE__ << "   " << eLog.data() << " " << std::hex << shaderType << std::endl;

        assert(0);
    }

    glAttachShader(theProgram, theShader);
}

void CompilerShaders(GLuint &shader)
{
    shader = glCreateProgram();
    assert(shader);

    AddShader(shader, vertexShaderText, GL_VERTEX_SHADER);
    AddShader(shader, fragmentShaderText, GL_FRAGMENT_SHADER);

    GLint result = 0;
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLint logLength;
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> eLog(logLength);
        glGetProgramInfoLog(shader, logLength, nullptr, eLog.data());

        std::cerr << __FUNCTION__ << ":" << __LINE__ << " " << eLog.data() << std::endl;

        assert(0);
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        GLint logLength;
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> eLog(logLength);
        glGetProgramInfoLog(shader, logLength, nullptr, eLog.data());

        std::cerr << __FUNCTION__ << ":" << __LINE__ << " " << eLog.data() << std::endl;

        assert(0);
    }
}

int main(int argc, char const *argv[])
{
    // Window dimensions
    constexpr GLint WIDTH = 800, HEIGHT = 600;

    assert(glfwInit());

    // Setup GLFW Window Properties
    // OpemGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow Forward Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(WIDTH, HEIGHT, "GL Win", nullptr, nullptr);
    assert(window != nullptr);

    // Set context for GLEW to use
    glfwMakeContextCurrent(window);

    // Get Buffer size information
    GLint bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    // Glew init
    assert(glewInit() == GLEW_OK);

    // Allow modern extension features
    glViewport(0, 0, bufferWidth, bufferHeight);

    GLuint VAO, VBO;
    CreateTriangle(VAO, VBO);

    GLuint program;
    CompilerShaders(program);

    glClearColor(0.2, 0.6, 0.8, 1.0);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // Clear window
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw

        glUseProgram(program);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
