#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "ShadowMap.hpp"

class Light
{
public:
    Light(GLuint shadowWidth = 1024, GLuint shadowHeight = 1024,
          GLfloat red = 1.f, GLfloat green = 1.f, GLfloat blue = 1.f,
          GLfloat ambIntensity = 1.f, GLfloat diffuseIntensity = 0.f);

    virtual void UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                          GLint diffuseIntensityLocation);

    ShadowMap *GetShadowMap() { return mShadowMap; }

    virtual ~Light();

protected:
    glm::vec3 mColor;
    GLfloat mAmbientIntensity;
    GLfloat mDiffuseIntensity;

    glm::mat4 mLightProj{1.f};
    ShadowMap *mShadowMap;
};
