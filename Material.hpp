#pragma once

#include <GL/glew.h>

class Material
{
public:
    Material(GLfloat specularIntensity = 0.f, GLfloat shininess = 0.f);

    void UseMaterial(GLint specularIntensityLocation, GLint shininessLocation);

    ~Material();

private:
    GLfloat mSpecularIntensity;
    GLfloat mShininess;
};
