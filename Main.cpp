#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

int CheckGLError()
{
    GLenum errCode;
    const GLubyte *errString;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        std::cerr << errString << "\n";

        return -1;
    }

    return 0;
}

void CreateObjects(std::vector<Mesh *> &meshList)
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2};

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
}

void CreateShaders(std::vector<Shader> &shaderList)
{
    Shader *shader = new Shader;
    shader->CreateShaderFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
    shaderList.push_back(*shader);
}

int main(int argc, char const *argv[])
{
    Window *wind = new Window(800, 600);
    wind->Initialize();

    std::vector<Mesh *> meshList;
    std::vector<Shader> shaderList;

    CreateObjects(meshList);
    CreateShaders(shaderList);

    shaderList[0].UseShader();
    while(CheckGLError() != 0);
    GLuint uniformProjection = shaderList[0].GetProjectionLocation();
    GLuint uniformModel = shaderList[0].GetModelLocation();

    std::pair<GLint, GLint> bufferDims = wind->GetBufferDim();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)bufferDims.first / bufferDims.second, 0.1f, 100.0f);

    glClearColor(0.2, 0.6, 0.8, 1.0);
    GLfloat angle = 0;
    while (!wind->ShouldWindowClose())
    {
        angle += 0.01;
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();

        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(0.5f * cosf(angle), 0.5f * sinf(angle), -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f * sinf(angle), 0.5f * cosf(angle), -2.5f));
        model = glm::rotate(model, angle, glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

        wind->SwapBuffers();
    }

    delete wind;
    return 0;
}
