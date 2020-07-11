#pragma once

#include <iostream>
#include <array>

#include "Light.hpp"
#include "OmniShadowMap.hpp"

class PointLight : public Light
{
public:
    PointLight(GLsizei shadowWidth, GLsizei shadowHeight,
               GLfloat near, GLfloat far,
               GLfloat red, GLfloat green, GLfloat blue,
               GLfloat ambIntensity, GLfloat diffuseIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat quad);

    void UseLight(GLint ambIntensityLocation, GLint colorLocation,
                  GLint diffuseIntensityLocation, GLint positionLocation,
                  GLint constantLocation, GLint linearLocation, GLint quadraticLocation);

    std::array<glm::mat4, 6> CalculateLightTransform();

    GLfloat GetFarPlane() { return mFarPlane; }
    const glm::vec3 &GetPosition() { return mPosition; }

    PointLight(const PointLight&) = delete;
    PointLight& operator=(const PointLight&) = delete;

    virtual ~PointLight();

protected:
    glm::vec3 mPosition;

    GLfloat mConstant, mLinear, mQuadratic;

    GLfloat mFarPlane;
};
