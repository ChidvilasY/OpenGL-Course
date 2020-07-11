#pragma once

#include <GL/glew.h>

class Material
{
public:
    Material(GLfloat specularIntensity, GLfloat shininess);

    void UseMaterial(GLint specularIntensityLocation, GLint shininessLocation);

    ~Material();

private:
    GLfloat mSpecularIntensity;
    GLfloat mShininess;
};
