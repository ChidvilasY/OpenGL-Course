#include "Light.hpp"


Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity)
    : mColor(glm::vec3(red, green, blue)),
      mAmbientIntensity(ambIntensity)
{
}

void Light::UseLight(GLint ambIntensityLocation, GLint ambColorLocation)
{
    glUniform3f(ambColorLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(ambIntensityLocation, mAmbientIntensity);
}

Light::~Light()
{
}
