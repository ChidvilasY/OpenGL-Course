#pragma once

#include "Light.hpp"

class PointLight : public Light
{
public:
    PointLight(GLfloat red = 1.f, GLfloat green = 1.f, GLfloat blue = 1.f,
               GLfloat ambIntensity = 1.f, GLfloat diffuseIntensity = 0.f,
               GLfloat xPos = 0.f, GLfloat yPos = -1.f, GLfloat zPos = 0.f,
               GLfloat con = 1.f, GLfloat lin = 0.f, GLfloat quad = 0.f);

    void UseLight(GLint ambIntensityLocation, GLint colorLocation,
                  GLint diffuseIntensityLocation, GLint positionLocation,
                  GLint constantLocation, GLint linearLocation, GLint quadraticLocation);

    virtual ~PointLight();

protected:
    glm::vec3 mPosition;

    GLfloat mConstant, mLinear, mQuadratic;
};
