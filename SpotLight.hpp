#pragma once

#include "PointLight.hpp"

class SpotLight : public PointLight
{
public:
    SpotLight(GLfloat red = 1.f, GLfloat green = 1.f, GLfloat blue = 1.f,
              GLfloat ambIntensity = 1.f, GLfloat diffuseIntensity = 0.f,
              GLfloat xPos = 0.f, GLfloat yPos = -1.f, GLfloat zPos = 0.f,
              GLfloat xDir = 0.f, GLfloat yDir = -1.f, GLfloat zDir = 0.f,
              GLfloat con = 1.f, GLfloat lin = 0.f, GLfloat quad = 0.f, GLfloat edge = 0.5f);

    void UseLight(GLint ambIntensityLocation, GLint colorLocation,
                  GLint diffuseIntensityLocation, GLint positionLocation, GLint directionLocation,
                  GLint constantLocation, GLint linearLocation, GLint quadraticLocation,
                  GLint edgeLocation);

    void SetFlash(const glm::vec3 &pos, const glm::vec3 &dir);

    virtual ~SpotLight();

private:
    glm::vec3 mDirection;
    GLfloat mEdge, mProcessedEdge;
};
