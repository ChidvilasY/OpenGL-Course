#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

class Shader
{
public:
    Shader();

    void CreateShaderFromString(const char *vertexCode, const char *fragmentCode);
    void CreateShaderFromFiles(const char *vertexLocation, const char *fragmentLocation);

    std::vector<char> ReadFile(const std::string fileName);

    GLint GetProjectionLocation()
    {
        return mUniformProjection;
    }

    GLint GetModelLocation()
    {
        return mUniformModel;
    }

    GLint GetViewLocation()
    {
        return mUniformView;
    }

    GLint GetAmbientIntensityLocation()
    {
        return mUniforomAmbientIntensity;
    }

    GLint GetAmbientColorLocation()
    {
        return mUniformAmbientColor;
    }

    void UseShader()
    {
        glUseProgram(mShaderID);
    }

    void ClearShader();

    ~Shader();

private:
    GLuint mShaderID;
    GLint mUniformProjection, mUniformModel, mUniformView;
    GLint mUniforomAmbientIntensity, mUniformAmbientColor;

    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void AddShader(GLuint program, const char *shaderCode, GLenum shaderType);
};
