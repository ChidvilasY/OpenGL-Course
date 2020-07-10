#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Light.hpp"

class DirectionalLight : public Light
{
public:
    DirectionalLight(GLuint shadowWidth = 1024, GLuint shadowHeight = 1024,
                     GLfloat red = 1.f, GLfloat green = 1.f, GLfloat blue = 1.f,
                     GLfloat ambIntensity = 1.f, GLfloat diffuseIntensity = 0.f,
                     GLfloat xDir = 0.f, GLfloat yDir = -1.f, GLfloat zDir = 0.f);

    void UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                  GLint diffuseIntensityLocation, GLint diffuseDirection);


    glm::mat4 CalculateLightTransform();

    virtual ~DirectionalLight();

private:
    glm::vec3 mDirection;
};
