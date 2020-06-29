#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "CommonValues.hpp"

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

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
        return mUniformDirectionLight.uniformAmbientIntensity;
    }

    GLint GetAmbientColorLocation()
    {
        return mUniformDirectionLight.uniformColor;
    }

    GLint GetDirectionLocation()
    {
        return mUniformDirectionLight.uniformDirection;
    }

    GLint GetDiffuseIntensityLocation()
    {
        return mUniformDirectionLight.uniformDiffuseIntensity;
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

    void SetDirectionalLight(DirectionalLight *dirLight);
    void SetPointLights(PointLight *poiLight, unsigned int lightCount);
    void SetSpotLights(SpotLight *spotLight, unsigned int lightCount);

    void ClearShader();

    ~Shader();

private:
    GLuint mShaderID;

    int mPointLightCount{}, mSpotLightCount{};

    GLint mUniformProjection, mUniformModel, mUniformView, mEyePosLocation;
    GLint mUniformSpecularIntensity, mUniformShininess;
    GLint mUniformPointLightCount, mUniformSpotLightCount;

    struct
    {
        GLint uniformColor;
        GLint uniformAmbientIntensity;
        GLint uniformDiffuseIntensity;

        GLint uniformDirection;
    } mUniformDirectionLight;

    struct UniformPointLight
    {
        GLint uniformColor;
        GLint uniformAmbientIntensity;
        GLint uniformDiffuseIntensity;

        GLint uniformPosition;
        GLint uniformConstant;
        GLint uniformLinear;
        GLint uniformQuadratic;
    };

    struct UniformSpotLight
    {
        GLint uniformColor;
        GLint uniformAmbientIntensity;
        GLint uniformDiffuseIntensity;

        GLint uniformPosition;
        GLint uniformConstant;
        GLint uniformLinear;
        GLint uniformQuadratic;

        GLint uniformDirection;
        GLint uniformEdge;
    };

    UniformPointLight mUniformPointLights[MAX_POINT_LIGHTS];
    UniformSpotLight mUniformSpotLights[MAX_SPOT_LIGHTS];

    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void AddShader(GLuint program, const char *shaderCode, GLenum shaderType);

    GLint GetUniformLocation(const char *uniformName);
};
