#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.hpp"

Shader::Shader()
    : mShaderID(0)
{
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CreateShaderFromString(const char *vertexCode, const char *fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateShaderFromFiles(const char *vertexLocation, const char *fragmentLocation)
{
    std::vector<char> vertexString = ReadFile(vertexLocation);
    std::vector<char> fragmentString = ReadFile(fragmentLocation);

    CompileShader(vertexString.data(), fragmentString.data());
}

void Shader::CreateShaderFromFiles(const char *vertexLocation, const char *geometryLocation, const char *fragmentLocation)
{
    std::vector<char> vertexString = ReadFile(vertexLocation);
    std::vector<char> fragmentString = ReadFile(fragmentLocation);
    std::vector<char> geometryString = ReadFile(geometryLocation);

    CompileShader(vertexString.data(), geometryString.data(), fragmentString.data());
}

void Shader::CompileProgram()
{
    GLint result = 0;
    glLinkProgram(mShaderID);
    glGetProgramiv(mShaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLint logLength;
        glGetProgramiv(mShaderID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> eLog(static_cast<std::size_t>(logLength));
        glGetProgramInfoLog(mShaderID, logLength, nullptr, eLog.data());

        throw std::runtime_error(eLog.data());
    }

    mUniformModel = GetUniformLocation("model");
    mUniformProjection = GetUniformLocation("projection");
    mUniformView = GetUniformLocation("view");

    mUniformDirectionLight.uniformAmbientIntensity = GetUniformLocation("directionalLight.base.ambientIntensity");
    mUniformDirectionLight.uniformColor = GetUniformLocation("directionalLight.base.color");

    mUniformDirectionLight.uniformDiffuseIntensity = GetUniformLocation("directionalLight.base.diffuseIntensity");
    mUniformDirectionLight.uniformDirection = GetUniformLocation("directionalLight.direction");

    mUniformShininess = GetUniformLocation("material.shininess");
    mUniformSpecularIntensity = GetUniformLocation("material.specularIntensity");

    mEyePosLocation = GetUniformLocation("eyePosition");

    mUniformPointLightCount = GetUniformLocation("pointLightCount");
    mUniformSpotLightCount = GetUniformLocation("spotLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        std::string uniStr;

        uniStr = "pointLights[" + std::to_string(i) + "].base.color";
        mUniformPointLights[i].uniformColor = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].base.ambientIntensity";
        mUniformPointLights[i].uniformAmbientIntensity = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].base.diffuseIntensity";
        mUniformPointLights[i].uniformDiffuseIntensity = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].position";
        mUniformPointLights[i].uniformPosition = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].constant";
        mUniformPointLights[i].uniformConstant = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].linear";
        mUniformPointLights[i].uniformLinear = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].quadratic";
        mUniformPointLights[i].uniformQuadratic = GetUniformLocation(uniStr.data());
    }

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        std::string uniStr;

        uniStr = "spotLights[" + std::to_string(i) + "].base.base.color";
        mUniformSpotLights[i].uniformColor = GetUniformLocation(uniStr.data());

        uniStr = "spotLights[" + std::to_string(i) + "].base.base.ambientIntensity";
        mUniformSpotLights[i].uniformAmbientIntensity = GetUniformLocation(uniStr.data());

        uniStr = "spotLights[" + std::to_string(i) + "].base.base.diffuseIntensity";
        mUniformSpotLights[i].uniformDiffuseIntensity = GetUniformLocation(uniStr.data());

        uniStr = "spotLights[" + std::to_string(i) + "].base.position";
        mUniformSpotLights[i].uniformPosition = GetUniformLocation(uniStr.data());

        uniStr = "spotLights[" + std::to_string(i) + "].base.constant";
        mUniformSpotLights[i].uniformConstant = GetUniformLocation(uniStr.data());

        uniStr = "spotLights[" + std::to_string(i) + "].base.linear";
        mUniformSpotLights[i].uniformLinear = GetUniformLocation(uniStr.data());

        uniStr = "spotLights[" + std::to_string(i) + "].base.quadratic";
        mUniformSpotLights[i].uniformQuadratic = GetUniformLocation(uniStr.data());

        uniStr = "spotLights[" + std::to_string(i) + "].direction";
        mUniformSpotLights[i].uniformDirection = GetUniformLocation(uniStr.data());

        uniStr = "spotLights[" + std::to_string(i) + "].edge";
        mUniformSpotLights[i].uniformEdge = GetUniformLocation(uniStr.data());
    }

    mUniformTexture = GetUniformLocation("theTexture");
    mUniformDirectionalLightTransform = GetUniformLocation("directionalLightSpaceTransform");
    mUniformDirectionalShadowMap = GetUniformLocation("directionalShadowMap");

    mUniformFarPlane = GetUniformLocation("farPlane");
    mUnifromLightPos = GetUniformLocation("lightPos");

    for (size_t i = 0; i < 6; i++)
    {
        std::string uniStr;
        uniStr = "lightMatrices[" + std::to_string(i) + "]";

        mUniformLightMatrices[i] = GetUniformLocation(uniStr.c_str());
    }

    for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
    {
        std::string uniStr;

        uniStr = "omniShadowMaps[" + std::to_string(i) + "].shadowMap";
        mUniformOmniShadowMap[i].uniformShadowMap = GetUniformLocation(uniStr.data());

        uniStr = "omniShadowMaps[" + std::to_string(i) + "].farPlane";
        mUniformOmniShadowMap[i].uniformFarPlane = GetUniformLocation(uniStr.data());
    }
}

void Shader::Validate()
{
    GLint result;
    glValidateProgram(mShaderID);
    glGetProgramiv(mShaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        GLint logLength;
        glGetProgramiv(mShaderID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> eLog(static_cast<std::size_t>(logLength));
        glGetProgramInfoLog(mShaderID, logLength, nullptr, eLog.data());

        throw std::runtime_error(eLog.data());
    }
}

std::vector<char> Shader::ReadFile(const std::string fileName)
{
    // Open stream from given file
    // std::ios::binary tells stream to read file as binary
    // std::ios::ate tells stream start reading from end of file
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);

    // Check if file stream successfully opened
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    // Get current read position and use to resize file buffer
    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> fileBuffer(fileSize);

    // Move read position to start of the file
    file.seekg(0);

    // Read file data into the buffer
    file.read(fileBuffer.data(), static_cast<std::streamsize>(fileSize));

    // Close the file
    file.close();

    fileBuffer.push_back(0);

    return fileBuffer;
}

void Shader::ClearShader()
{
    if (mShaderID)
    {
        glDeleteProgram(mShaderID);
        mShaderID = 0;
    }

    mUniformModel = 0;
    mUniformProjection = 0;
}

void Shader::CompileShader(const char *vertexCode, const char *geometryCode, const char *fragmentCode)
{
    mShaderID = glCreateProgram();
    if (mShaderID == 0)
    {
        throw std::runtime_error("Failed to Create GL Program");
    }

    AddShader(mShaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(mShaderID, geometryCode, GL_GEOMETRY_SHADER);
    AddShader(mShaderID, fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram();
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode)
{
    mShaderID = glCreateProgram();
    if (mShaderID == 0)
    {
        throw std::runtime_error("Failed to Create GL Program");
    }

    AddShader(mShaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(mShaderID, fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram();
}

GLint Shader::GetUniformLocation(const char *uniformName)
{
    GLint loc = glGetUniformLocation(mShaderID, uniformName);
    if (loc < 0)
    {
        std::cerr << "Failed to Get Unifrom Location: " << uniformName << std::endl;
    }

    return loc;
}

void Shader::AddShader(GLuint program, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar *theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1] = {static_cast<GLint>(strlen(shaderCode))};

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result;
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLint logLength;
        glGetShaderiv(theShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> eLog(static_cast<std::size_t>(logLength));
        glGetShaderInfoLog(theShader, logLength, nullptr, eLog.data());

        throw std::runtime_error(eLog.data());
    }

    glAttachShader(program, theShader);
}

void Shader::SetDirectionalLight(DirectionalLight *dirLight)
{
    dirLight->UseLight(mUniformDirectionLight.uniformAmbientIntensity,
                       mUniformDirectionLight.uniformColor,
                       mUniformDirectionLight.uniformDiffuseIntensity,
                       mUniformDirectionLight.uniformDirection);
}

void Shader::SetPointLights(PointLight *poiLights, GLuint lightCount, GLuint textureUnit, GLuint offset)
{
    if (lightCount > MAX_POINT_LIGHTS)
    {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(mUniformPointLightCount, lightCount);

    for (GLuint i = 0; i < lightCount; i++)
    {
        poiLights[i].UseLight(mUniformPointLights[i].uniformAmbientIntensity,
                              mUniformPointLights[i].uniformColor,
                              mUniformPointLights[i].uniformDiffuseIntensity,
                              mUniformPointLights[i].uniformPosition,
                              mUniformPointLights[i].uniformConstant,
                              mUniformPointLights[i].uniformLinear,
                              mUniformPointLights[i].uniformQuadratic);

        poiLights[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(mUniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit + i);
        glUniform1f(mUniformOmniShadowMap[i + offset].uniformFarPlane, poiLights[i].GetFarPlane());
    }
}

void Shader::SetPointLights(const std::vector<PointLight *> &poiLights, GLuint textureUnit, GLuint offset)
{
    GLuint lightCount = static_cast<GLuint>(poiLights.size());

    if (lightCount > MAX_POINT_LIGHTS)
    {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(mUniformPointLightCount, lightCount);

    for (GLuint i = 0; i < lightCount; i++)
    {
        poiLights[i]->UseLight(mUniformPointLights[i].uniformAmbientIntensity,
                               mUniformPointLights[i].uniformColor,
                               mUniformPointLights[i].uniformDiffuseIntensity,
                               mUniformPointLights[i].uniformPosition,
                               mUniformPointLights[i].uniformConstant,
                               mUniformPointLights[i].uniformLinear,
                               mUniformPointLights[i].uniformQuadratic);

        poiLights[i]->GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(mUniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit + i);
        glUniform1f(mUniformOmniShadowMap[i + offset].uniformFarPlane, poiLights[i]->GetFarPlane());
    }
}

void Shader::SetSpotLights(const std::vector<SpotLight *> &spotLights, GLuint textureUnit, GLuint offset)
{
    GLuint lightCount = static_cast<GLuint>(spotLights.size());

    if (lightCount > MAX_SPOT_LIGHTS)
    {
        lightCount = MAX_SPOT_LIGHTS;
    }

    glUniform1i(mUniformSpotLightCount, lightCount);

    for (GLuint i = 0; i < lightCount; i++)
    {
        spotLights[i]->UseLight(mUniformSpotLights[i].uniformAmbientIntensity,
                              mUniformSpotLights[i].uniformColor,
                              mUniformSpotLights[i].uniformDiffuseIntensity,
                              mUniformSpotLights[i].uniformPosition,
                              mUniformSpotLights[i].uniformDirection,
                              mUniformSpotLights[i].uniformConstant,
                              mUniformSpotLights[i].uniformLinear,
                              mUniformSpotLights[i].uniformQuadratic,
                              mUniformSpotLights[i].uniformEdge);

        spotLights[i]->GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(mUniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit + i);
        glUniform1f(mUniformOmniShadowMap[i + offset].uniformFarPlane, spotLights[i]->GetFarPlane());
    }
}

void Shader::SetSpotLights(SpotLight *spotLight, GLuint lightCount, GLuint textureUnit, GLuint offset)
{
    if (lightCount > MAX_SPOT_LIGHTS)
    {
        lightCount = MAX_SPOT_LIGHTS;
    }

    glUniform1i(mUniformSpotLightCount, lightCount);

    for (GLuint i = 0; i < lightCount; i++)
    {
        spotLight[i].UseLight(mUniformSpotLights[i].uniformAmbientIntensity,
                              mUniformSpotLights[i].uniformColor,
                              mUniformSpotLights[i].uniformDiffuseIntensity,
                              mUniformSpotLights[i].uniformPosition,
                              mUniformSpotLights[i].uniformDirection,
                              mUniformSpotLights[i].uniformConstant,
                              mUniformSpotLights[i].uniformLinear,
                              mUniformSpotLights[i].uniformQuadratic,
                              mUniformSpotLights[i].uniformEdge);

        spotLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(mUniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit + i);
        glUniform1f(mUniformOmniShadowMap[i + offset].uniformFarPlane, spotLight[i].GetFarPlane());
    }
}

void Shader::SetTexture(GLuint textureUnit)
{
    glUniform1i(mUniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
    glUniform1i(mUniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(const glm::mat4 &lightTransform)
{
    glUniformMatrix4fv(mUniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(lightTransform));
}

void Shader::SetOmniLightTransfomMatrices(std::array<glm::mat4, 6> lightMatTransforms)
{
    for (GLsizei i = 0; i < 6; i++)
    {
        glUniformMatrix4fv(mUniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatTransforms[static_cast<std::size_t>(i)]));
    }
}
