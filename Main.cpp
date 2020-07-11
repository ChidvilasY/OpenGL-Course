#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>
#include <memory>

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
#include "SpotLight.hpp"
#include "Model.hpp"

GLint uniformProjection;
GLint uniformModel;
GLint uniformView;
GLint uniformSpecularIntensity;
GLint uniformShininess;
GLint uniformEyePos;
GLint uniformOmniLightPos;
GLint uniformFarPlane;
Model uh60;
GLfloat angle = 0;

std::vector<std::unique_ptr<Mesh>> meshList;
std::vector<Shader *> shaderList;
Camera *cam;

std::vector<PointLight *> pointLights;
std::vector<SpotLight *> spotLights;
DirectionalLight *dirLight;

bool renderPass = false;

Texture brickTexture("Textures/brick.png");
Texture dirtTexture("Textures/dirt.png");
Texture plainTexture("Textures/plain.jpg");
Material shinyMaterial(1.0f, 256);
Material dullMaterial(0.3f, 4);

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

void IgnoreErrorsUntilHere()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        std::cerr << "Ignorring: " << errString << "\n";
    }
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

void CreateObjects(std::vector<std::unique_ptr<Mesh>> &meshList)
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
        // x y z u v nx ny
        -20.f, 0, -20.f, 0.f, 0.f, 0.f, -1.f, 0.f,
        20.f, 0, -20.f, 10.f, 0.f, 0.f, -1.f, 0.f,
        -20.f, 0, 20.f, 0.f, 10.f, 0.f, -1.f, 0.f,
        20.f, 0, 20.f, 10.f, 10.f, 0.f, -1.f, 0.f};

    CalcAverageNormals(indices, 12, vertices, 32, 8, 5);
    // CalcAverageNormals(indices, 12, planeVertices, 32, 8, 5);

    std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(std::move(obj1));

    std::unique_ptr<Mesh> obj2 = std::make_unique<Mesh>();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(std::move(obj2));

    std::unique_ptr<Mesh> obj3 = std::make_unique<Mesh>();
    obj3->CreateMesh(planeVertices, indices, 32, 12);
    meshList.push_back(std::move(obj3));
}

void CreateShaders(std::vector<Shader *> &shaderList, Shader *&directionalShadowShader, Shader *&omniShadowMapShader)
{
    Shader *shader = new Shader;
    shader->CreateShaderFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
    shaderList.push_back(shader);

    directionalShadowShader = new Shader();
    directionalShadowShader->CreateShaderFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");

    omniShadowMapShader = new Shader();
    omniShadowMapShader->CreateShaderFromFiles("Shaders/omni_directional_shadow_map.vert",
                                               "Shaders/omni_directional_shadow_map.geom",
                                               "Shaders/omni_directional_shadow_map.frag");
}

void DeleteShaders(std::vector<Shader *> &shaderList, Shader *&directionalShadowShader, Shader *&omniShadowMapShader)
{
    for (auto &&shader : shaderList)
    {
        delete shader;
    }

    shaderList.clear();
    delete directionalShadowShader;
    delete omniShadowMapShader;
}

void RenderScene()
{
    glm::mat4 modelMat(1.0f);

    modelMat = glm::translate(modelMat, glm::vec3(0.5f * cosf(angle), 0.5f * sinf(angle), -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));

    if (renderPass)
    {
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        brickTexture.UseTexture();
    }

    meshList[0]->RenderMesh();

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(2.0f * cosf(angle), 4.0f, 0.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));

    if (renderPass)
    {
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        dirtTexture.UseTexture();
    }

    meshList[1]->RenderMesh();

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, -3.0f, 0.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));
    if (renderPass)
    {
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        dirtTexture.UseTexture();
    }
    meshList[2]->RenderMesh();

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(20.0f, 17.0f, 0.f));
    modelMat = glm::rotate(modelMat, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));
    if (renderPass)
    {
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        plainTexture.UseTexture();
    }
    meshList[2]->RenderMesh();

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(-20.0f, 17.0f, 0.f));
    modelMat = glm::rotate(modelMat, glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));
    if (renderPass)
    {
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        plainTexture.UseTexture();
    }
    meshList[2]->RenderMesh();

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 17.0f, 20.f));
    modelMat = glm::rotate(modelMat, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));
    if (renderPass)
    {
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        plainTexture.UseTexture();
    }
    meshList[2]->RenderMesh();

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 17.0f, -20.f));
    modelMat = glm::rotate(modelMat, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));
    if (renderPass)
    {
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        plainTexture.UseTexture();
    }
    meshList[2]->RenderMesh();

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 37.0f, 0.f));
    modelMat = glm::rotate(modelMat, glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));
    if (renderPass)
    {
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        plainTexture.UseTexture();
    }
    meshList[2]->RenderMesh();

    modelMat = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, -angle, glm::vec3(0.f, 1.f, 0.f));
    modelMat = glm::translate(modelMat, glm::vec3(-8.0f, 2.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::scale(modelMat, glm::vec3(0.4f, 0.4f, 0.4f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));
    if (renderPass)
    {
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    }
    uh60.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight *light, Shader *directionalShadowShader)
{
    directionalShadowShader->UseShader();

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader->GetModelLocation();
    directionalShadowShader->SetDirectionalLightTransform(light->CalculateLightTransform());

    directionalShadowShader->Validate();

    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight *light, Shader *omniShadowMapShader)
{
    omniShadowMapShader->UseShader();

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

    uniformModel = omniShadowMapShader->GetModelLocation();
    uniformOmniLightPos = omniShadowMapShader->GetOmniLightPosLocation();
    uniformFarPlane = omniShadowMapShader->GetFarPlaneLocation();

    light->GetShadowMap()->Write();

    glClear(GL_DEPTH_BUFFER_BIT);

    glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());
    omniShadowMapShader->SetOmniLightTransfomMatrices(light->CalculateLightTransform());

    omniShadowMapShader->Validate();

    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(Window &wind)
{
    shaderList[0]->UseShader();

    uniformProjection = shaderList[0]->GetProjectionLocation();
    uniformModel = shaderList[0]->GetModelLocation();
    uniformView = shaderList[0]->GetViewLocation();
    uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
    uniformShininess = shaderList[0]->GetShininessLocation();
    uniformEyePos = shaderList[0]->GetEyePosLocation();

    std::pair<GLint, GLint> bufferDims = wind.GetBufferDim();
    GLfloat aspectRatio = static_cast<GLfloat>(bufferDims.first) / static_cast<GLfloat>(bufferDims.second);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cam->CalculateViewMatrix()));
    glUniform3f(uniformEyePos, cam->GetCameraPosition().x, cam->GetCameraPosition().y, cam->GetCameraPosition().z);

    glViewport(0, 0, bufferDims.first, bufferDims.second);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 lowerSpotLight = cam->GetCameraPosition();
    lowerSpotLight.y -= (0.3f + 0.01f * sinf(angle * 4));
    lowerSpotLight.x -= (0.05f * sinf(angle * 5));

    GLuint pointLightCount = static_cast<GLuint>(pointLights.size());

    shaderList[0]->SetDirectionalLight(dirLight);
    shaderList[0]->SetPointLights(pointLights, 3, 0);
    shaderList[0]->SetSpotLights(spotLights, 3 + pointLightCount, pointLightCount);
    shaderList[0]->SetDirectionalLightTransform(dirLight->CalculateLightTransform());

    shaderList[0]->SetTexture(1);
    shaderList[0]->SetDirectionalShadowMap(2);

    // spotLights[0]->SetFlash(lowerSpotLight, cam->GetCameraDirection());
    dirLight->GetShadowMap()->Read(GL_TEXTURE2);

    shaderList[0]->Validate();

    renderPass = true;
    RenderScene();
    renderPass = false;
}

int main(int argc, char const *argv[])
{
    Window wind(1366, 768);
    wind.Initialize();

    Shader *directionalShadowShader = nullptr;
    Shader *omniShadowMapShader = nullptr;
    CreateShaders(shaderList, directionalShadowShader, omniShadowMapShader);

    CreateObjects(meshList);
    uh60.LoadModel("Models/uh60.obj");

    brickTexture.LoadTextureA();
    dirtTexture.LoadTextureA();
    if (!plainTexture.LoadTexture())
    {
        std::cerr << "Loading failed"
                  << "\n";
        return -1;
    }

    cam = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

    dirLight = new DirectionalLight(2048, 2048,
                                    1.0f, 1.0f, 1.0f,
                                    0.0f, 0.0f,
                                    0.0f, -15.0f, -10.0f);

    pointLights.push_back(new PointLight(1024, 1024, 0.1f, 100.0f,
                                         1.f, .0f, 0.0f,
                                         0.f, 0.4f,
                                         -3.f, 2.0f, 0.0f,
                                         0.3f, 0.01f, 0.01f));

    pointLights.push_back(new PointLight(1024, 1024, 0.01f, 100.f,
                                         0.0f, 1.0f, 0.0f,
                                         0.0f, 0.4f,
                                         0.0f, 2.0f, 0.0f,
                                         0.3f, 0.01f, 0.01f));

    spotLights.push_back(new SpotLight(1024, 1024, 0.1f, 100.0f,
                                       1.0f, 1.0f, 1.0f,
                                       0.0f, 0.8f,
                                       0.0f, 0.0f, 0.0f,
                                       0.0f, -1.0f, 0.0f,
                                       1.0f, 0.0f, 0.0f,
                                       20.0f));

    spotLights.push_back(new SpotLight(1024, 1024, 0.01f, 1000.f,
                                       1.f, 1.0f, 1.f,
                                       0.f, 1.0f,
                                       0.f, 0.f, 0.f,
                                       0.f, -1.f, 0.f,
                                       1.f, 0.f, 0.f,
                                       20.f));

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    GLfloat deltaTime, lastTime = 0;
    while (!wind.ShouldWindowClose())
    {
        GLfloat now = static_cast<GLfloat>(glfwGetTime()); // SDL_GetPerformanceCounter();
        deltaTime = (now - lastTime);                      // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
        lastTime = now;
        angle += deltaTime;

        std::cout << 1. / deltaTime << std::endl;

        glfwPollEvents();

        cam->KeyControl(wind.GetKeyStates(), deltaTime);
        cam->MouseContol(wind.GetCursorChangeX(), wind.GetCursorChangeY());

        DirectionalShadowMapPass(dirLight, directionalShadowShader);

        for (size_t i = 0; i < pointLights.size(); i++)
        {
            OmniShadowMapPass(pointLights[i], omniShadowMapShader);
        }

        for (size_t i = 0; i < spotLights.size(); i++)
        {
            OmniShadowMapPass(spotLights[i], omniShadowMapShader);
        }

        RenderPass(wind);

        wind.SwapBuffers();
    }

    DeleteShaders(shaderList, directionalShadowShader, omniShadowMapShader);

    for (auto &&pointLight : pointLights)
    {
        delete pointLight;
    }

    pointLights.clear();

    for (auto &&spotLight : spotLights)
    {
        delete spotLight;
    }

    spotLights.clear();

    return 0;
}
