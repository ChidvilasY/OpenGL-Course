#include "Material.hpp"

Material::Material(GLfloat specularIntensity, GLfloat shininess)
    : mSpecularIntensity(specularIntensity),
      mShininess(shininess)
{
}

Material::~Material()
{
}

void Material::UseMaterial(GLint specularIntensityLocation, GLint shininessLocation)
{
    glUniform1f(specularIntensityLocation, mSpecularIntensity);
    glUniform1f(shininessLocation, mShininess);
}
