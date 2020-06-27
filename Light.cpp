#include "Light.hpp"

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseIntensity)
    : mColor(glm::vec3(red, green, blue)),
      mAmbientIntensity(ambIntensity),
      mDirection(glm::vec3(xDir, yDir, zDir)),
      mDiffuseIntensity(diffuseIntensity)
{
}

void Light::UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                     GLint diffuseIntensityLocation, GLint diffuseDirection)
{
    glUniform3f(ambColorLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(ambIntensityLocation, mAmbientIntensity);

    glUniform3f(diffuseDirection, mDirection.x, mDirection.y, mDirection.z);
    glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
}

Light::~Light()
{
}
