#pragma once

#include "Light.hpp"

class DirectionalLight : public Light
{
public:
    DirectionalLight(GLfloat red = 1.f, GLfloat green = 1.f, GLfloat blue = 1.f,
                     GLfloat ambIntensity = 1.f, GLfloat diffuseIntensity = 0.f,
                     GLfloat xDir = 0.f, GLfloat yDir = -1.f, GLfloat zDir = 0.f);

    void UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                  GLint diffuseIntensityLocation, GLint diffuseDirection);

    virtual ~DirectionalLight();

private:
    glm::vec3 mDirection;
};
