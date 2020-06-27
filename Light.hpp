#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light(GLfloat red = 1.f, GLfloat green = 1.f, GLfloat blue = 1.f, GLfloat ambIntensity = 1.f,
          GLfloat xDir = 0.f, GLfloat yDir = -1.f, GLfloat zDir = 0.f, GLfloat diffuseIntensity = 0.f);

    void UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                  GLint diffuseIntensityLocation, GLint diffuseDirection);

    ~Light();

private:
    glm::vec3 mColor;
    GLfloat mAmbientIntensity;

    glm::vec3 mDirection;
    GLfloat mDiffuseIntensity;
};
