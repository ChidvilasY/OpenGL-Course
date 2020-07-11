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
    void CreateShaderFromFiles(const char *vertexLocation, const char * geometryLocation, const char *fragmentLocation);

    void Validate();

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

    GLint GetOmniLightPosLocation()
    {
        return mUnifromLightPos;
    }

    GLint GetFarPlaneLocation()
    {
        return mUniformFarPlane;
    }

    void UseShader()
    {
        glUseProgram(mShaderID);
    }

    void SetDirectionalLight(DirectionalLight *dirLight);
    void SetPointLights(const std::vector<PointLight*> &poiLight, GLuint textureUnit, GLuint offset);
    void SetPointLights(PointLight *pointLight, GLuint lightCount, GLuint textureUnit, GLuint offset);
    void SetSpotLights(const std::vector<SpotLight *> &spotLights, GLuint textureUnit, GLuint offset);
    void SetSpotLights(SpotLight *spotLight, GLuint lightCount, GLuint textureUnit, GLuint offset);
    void SetTexture(GLuint textureUnit);
    void SetDirectionalShadowMap(GLuint textureUnit);
    void SetDirectionalLightTransform(const glm::mat4 &lightTransform);
    void SetOmniLightTransfomMatrices(std::array<glm::mat4, 6> lightMatTransforms);

    void ClearShader();

    ~Shader();

private:
    GLuint mShaderID;

    GLint mUniformProjection, mUniformModel, mUniformView, mEyePosLocation;
    GLint mUniformSpecularIntensity, mUniformShininess;
    GLint mUniformPointLightCount, mUniformSpotLightCount;
    GLint mUniformDirectionalLightTransform, mUniformDirectionalShadowMap;
    GLint mUniformTexture;
    GLint mUnifromLightPos, mUniformFarPlane;
    GLint mUniformLightMatrices[6];

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

    struct UniformOmniShadowMap
    {
        GLint uniformShadowMap;
        GLint uniformFarPlane;
    };

    UniformPointLight mUniformPointLights[MAX_POINT_LIGHTS];
    UniformSpotLight mUniformSpotLights[MAX_SPOT_LIGHTS];
    UniformOmniShadowMap mUniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS ];


    std::vector<char> ReadFile(const std::string fileName);
    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void CompileShader(const char *vertexCode, const char *geometryCode, const char *fragmentCode);
    void AddShader(GLuint program, const char *shaderCode, GLenum shaderType);

    void CompileProgram();

    GLint GetUniformLocation(const char *uniformName);
};
