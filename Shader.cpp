#include <fstream>

#include <cstring>
#include <cassert>

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

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        std::string uniStr;

        uniStr = "pointLights[" + std::to_string(i) + "].base.color";
        mUniformDPointLight[i].uniformColor = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].base.ambientIntensity";
        mUniformDPointLight[i].uniformAmbientIntensity = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].base.diffuseIntensity";
        mUniformDPointLight[i].uniformDiffuseIntensity = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].position";
        mUniformDPointLight[i].uniformPosition = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].constant";
        mUniformDPointLight[i].uniformConstant = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].linear";
        mUniformDPointLight[i].uniformLinear = GetUniformLocation(uniStr.data());

        uniStr = "pointLights[" + std::to_string(i) + "].quadratic";
        mUniformDPointLight[i].uniformQuadratic = GetUniformLocation(uniStr.data());
    }
}

GLint Shader::GetUniformLocation(const char *uniformName)
{
    GLint loc = glGetUniformLocation(mShaderID, uniformName);
    if (loc < 0)
    {
        throw std::runtime_error("Failed to Get Unifrom Location: " + std::string(uniformName));
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
        poiLight[i].UseLight(mUniformDPointLight[i].uniformAmbientIntensity,
                             mUniformDPointLight[i].uniformColor,
                             mUniformDPointLight[i].uniformDiffuseIntensity,
                             mUniformDPointLight[i].uniformPosition,
                             mUniformDPointLight[i].uniformConstant,
                             mUniformDPointLight[i].uniformLinear,
                             mUniformDPointLight[i].uniformQuadratic);
    }
}
