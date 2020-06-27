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
        return mUniformColor;
    }

    GLint GetDirectionLocation()
    {
        return mUniformDirection;
    }

    GLint GetDiffuseIntensityLocation()
    {
        return mUniformDiffuseIntensity;
    }

    GLint GetSpecularIntensityLocation()
    {
        return mUniformSpecularIntensity;
    }

    GLint GetShininessLocation()
    {
        return mUniformShininess;
    }

    GLint GetEyePosLocation()
    {
        return mEyePosLocation;
    }

    void UseShader()
    {
        glUseProgram(mShaderID);
    }

    void ClearShader();

    ~Shader();

private:
    GLuint mShaderID;
    GLint mUniformProjection, mUniformModel, mUniformView, mEyePosLocation;
    GLint mUniforomAmbientIntensity, mUniformColor;
    GLint mUniformDiffuseIntensity, mUniformDirection;
    GLint mUniformSpecularIntensity, mUniformShininess;

    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void AddShader(GLuint program, const char *shaderCode, GLenum shaderType);

    GLint GetUniformLocation(const char *uniformName);
};
