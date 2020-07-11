#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Light.hpp"

class DirectionalLight : public Light
{
public:
    DirectionalLight(GLsizei shadowWidth, GLsizei shadowHeight,
                     GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat ambIntensity, GLfloat diffuseIntensity,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir);

    void UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                  GLint diffuseIntensityLocation, GLint diffuseDirection);

    glm::mat4 CalculateLightTransform();

    virtual ~DirectionalLight();

private:
    glm::vec3 mDirection;
};
