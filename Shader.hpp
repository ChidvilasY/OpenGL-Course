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

    GLuint GetProjectionLocation()
    {
        return mUniformProjection;
    }

    GLuint GetModelLocation()
    {
        return mUniformModel;
    }

    void UseShader()
    {
        glUseProgram(mShaderID);
    }

    void ClearShader();

    ~Shader();

private:
    GLuint mShaderID, mUniformProjection, mUniformModel;

    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void AddShader(GLuint program, const char *shaderCode, GLenum shaderType);
};
