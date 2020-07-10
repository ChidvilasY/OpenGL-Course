#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

Shader::Shader()
    : mShaderID(0),
      mPointLightCount(0),
      mUniformProjection(0),
      mUniformModel(0)
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
    file.read(fileBuffer.data(), fileSize);

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

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode)
{
    mShaderID = glCreateProgram();
    if (mShaderID == 0)
    {
        throw std::runtime_error("Failed to Create GL Program");
    }

    AddShader(mShaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(mShaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    glLinkProgram(mShaderID);
    glGetProgramiv(mShaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLint logLength;
        glGetProgramiv(mShaderID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> eLog(logLength);
        glGetProgramInfoLog(mShaderID, logLength, nullptr, eLog.data());

        throw std::runtime_error(eLog.data());
    }

    glValidateProgram(mShaderID);
    glGetProgramiv(mShaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        GLint logLength;
        glGetProgramiv(mShaderID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> eLog(logLength);
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
}

GLint Shader::GetUniformLocation(const char *uniformName)
{
    GLint loc = glGetUniformLocation(mShaderID, uniformName);
    if (loc < 0)
    {
        // std::cerr << "Failed to Get Unifrom Location: " << uniformName << std::endl;
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
        std::vector<GLchar> eLog(logLength);
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

void Shader::SetPointLights(PointLight *poiLight, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS)
    {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(mUniformPointLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
    {
        poiLight[i].UseLight(mUniformPointLights[i].uniformAmbientIntensity,
                             mUniformPointLights[i].uniformColor,
                             mUniformPointLights[i].uniformDiffuseIntensity,
                             mUniformPointLights[i].uniformPosition,
                             mUniformPointLights[i].uniformConstant,
                             mUniformPointLights[i].uniformLinear,
                             mUniformPointLights[i].uniformQuadratic);
    }
}

void Shader::SetSpotLights(SpotLight *spotLight, unsigned int lightCount)
{
    if (lightCount > MAX_SPOT_LIGHTS)
    {
        lightCount = MAX_SPOT_LIGHTS;
    }

    glUniform1i(mUniformSpotLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
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
