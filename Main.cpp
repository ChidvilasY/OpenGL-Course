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
#include "Camera.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"

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

void CalcAverageNormals(unsigned int *indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticeCount,
                        unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;
        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    for (unsigned int i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects(std::vector<Mesh *> &meshList)
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2};
    GLfloat vertices[] = {
        //x  y  z   u  v   nx  ny  nz
        -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f};

    GLfloat planeVertices[] = {
        -10.f,
        0,
        -10.f,
        0.f,
        0.f,
        0.f,
        -1.f,
        0.f,
        10.f,
        0,
        -10.f,
        10.f,
        0.f,
        0.f,
        -1.f,
        0.f,
        -10.f,
        0,
        10.f,
        0.f,
        10.f,
        0.f,
        -1.f,
        0.f,
        10.f,
        0,
        10.f,
        10.f,
        10.f,
        0.f,
        -1.f,
        0.f,
    };

    CalcAverageNormals(indices, 12, vertices, 32, 8, 5);
    // CalcAverageNormals(indices, 12, planeVertices, 32, 8, 5);

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);

    Mesh *obj3 = new Mesh();
    obj3->CreateMesh(planeVertices, indices, 32, 12);
    meshList.push_back(obj3);
}

void CreateShaders(std::vector<Shader> &shaderList)
{
    Shader *shader = new Shader;
    shader->CreateShaderFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
    shaderList.push_back(*shader);
}

int main(int argc, char const *argv[])
{
    Window wind(1366, 768);
    wind.Initialize();

    std::vector<Mesh *> meshList;
    std::vector<Shader> shaderList;
    CreateObjects(meshList);
    CreateShaders(shaderList);

    Texture brickTexture("Textures/brick.png");
    brickTexture.LoadTexture();
    Texture dirtTexture("Textures/dirt.png");
    dirtTexture.LoadTexture();
    Texture plainTexture("Textures/plain.jpg");
    plainTexture.LoadTexture();

    Material shinyMaterial = Material(1.0f, 256);
    Material dullMaterial = Material(0.3f, 4);

    Camera cam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

    DirectionalLight dirLight = DirectionalLight(1.0f, 1.0f, 1.0f,
                                                 0.1f, 0.2f,
                                                 0.0f, 0.0f, -1.0f);

    PointLight pointLights[MAX_POINT_LIGHTS];

    unsigned int pointLightCount = 0;

    pointLights[0] = PointLight(0.f, 0.0f, 1.f,
                                0.1f, 0.4f,
                                4.f, 1.f, 0.f,
                                0.3f, 0.1f, 0.1f);
    pointLightCount++;

    pointLights[1] = PointLight(0.f, 1.0f, 0.f,
                                0.1f, 1.0f,
                                -4.f, 2.f, 0.f,
                                0.3f, 0.05f, 0.1f);
    pointLightCount++;

    pointLights[2] = PointLight(1.f, 0.0f, 0.f,
                                0.1f, 1.0f,
                                -2.f, 2.f, 0.f,
                                0.3f, 0.05f, 0.1f);
    pointLightCount++;

    shaderList[0].UseShader();
    GLint uniformProjection = shaderList[0].GetProjectionLocation();
    GLint uniformModel = shaderList[0].GetModelLocation();
    GLint uniformView = shaderList[0].GetViewLocation();
    GLint uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
    GLint uniformShininess = shaderList[0].GetShininessLocation();
    GLint uniformEyePos = shaderList[0].GetEyePosLocation();

    shaderList[0].SetDirectionalLight(&dirLight);
    shaderList[0].SetPointLights(&pointLights[0], pointLightCount);

    std::pair<GLint, GLint> bufferDims = wind.GetBufferDim();
    GLfloat aspectRatio = static_cast<GLfloat>(bufferDims.first) / static_cast<GLfloat>(bufferDims.second);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    GLfloat angle = 0;
    GLfloat deltaTime, lastTime;
    while (!wind.ShouldWindowClose())
    {
        GLfloat now = static_cast<GLfloat>(glfwGetTime()); // SDL_GetPerformanceCounter();
        deltaTime = now - lastTime;                        // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
        lastTime = now;

        angle += 0.01f;
        glfwPollEvents();
        cam.KeyControl(wind.GetKeyStates(), deltaTime);
        cam.MouseContol(wind.GetCursorChangeX(), wind.GetCursorChangeY());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cam.CalculateViewMatrix()));
        glUniform3f(uniformEyePos, cam.GetCameraPosition().x, cam.GetCameraPosition().y, cam.GetCameraPosition().z);

        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(0.5f * cosf(angle), 0.5f * sinf(angle), -2.5f));
        // model = glm::translate(model, glm::vec3(0.f, 0.5f, -2.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        brickTexture.UseTexture();
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
        // model = glm::rotate(model, angle, glm::vec3(0.f, 0.f, 1.f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        dirtTexture.UseTexture();
        meshList[1]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        plainTexture.UseTexture();
        meshList[2]->RenderMesh();

        wind.SwapBuffers();
    }

    return 0;
}
