#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light(GLfloat red = 1.f, GLfloat green = 1.f, GLfloat blue = 1.f, GLfloat ambIntensity = 1.f);

    void UseLight(GLint ambIntensityLocation, GLint ambColorLocation);

    ~Light();

private:
    glm::vec3 mColor;
    GLfloat mAmbientIntensity;
};
