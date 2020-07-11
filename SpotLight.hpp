#pragma once

#include "PointLight.hpp"

class SpotLight : public PointLight
{
public:
    SpotLight(GLsizei shadowWidth, GLsizei shadowHeight,
              GLfloat near, GLfloat far,
              GLfloat red, GLfloat green, GLfloat blue,
              GLfloat ambIntensity, GLfloat diffuseIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat con, GLfloat lin, GLfloat quad, GLfloat edge);

    void UseLight(GLint ambIntensityLocation, GLint colorLocation,
                  GLint diffuseIntensityLocation, GLint positionLocation, GLint directionLocation,
                  GLint constantLocation, GLint linearLocation, GLint quadraticLocation,
                  GLint edgeLocation);

    void SetFlash(const glm::vec3 &pos, const glm::vec3 &dir);

    virtual ~SpotLight();

private:
    glm::vec3 mDirection;
    GLfloat mProcessedEdge;
};
