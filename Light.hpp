#pragma once

#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "ShadowMap.hpp"

class Light
{
public:
    Light(GLsizei shadowWidth, GLsizei shadowHeight,
          GLfloat red, GLfloat green, GLfloat blue,
          GLfloat ambIntensity, GLfloat diffuseIntensity);

    void UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                  GLint diffuseIntensityLocation);

    ShadowMap *GetShadowMap() { return mShadowMap; }

    virtual ~Light();

protected:
    glm::vec3 mColor;
    GLfloat mAmbientIntensity;
    GLfloat mDiffuseIntensity;

    glm::mat4 mLightProj{1.f};
    ShadowMap *mShadowMap = nullptr;
};
