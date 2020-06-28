#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light(GLfloat red = 1.f, GLfloat green = 1.f, GLfloat blue = 1.f,
          GLfloat ambIntensity = 1.f, GLfloat diffuseIntensity = 0.f);

    virtual void UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                          GLint diffuseIntensityLocation);

    virtual ~Light();

protected:
    glm::vec3 mColor;
    GLfloat mAmbientIntensity;
    GLfloat mDiffuseIntensity;
};
