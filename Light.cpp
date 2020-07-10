#include "Light.hpp"

Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat diffuseIntensity)
    : mColor(glm::vec3(red, green, blue)),
      mAmbientIntensity(ambIntensity),
      mDiffuseIntensity(diffuseIntensity),
      mShadowMap(new ShadowMap())
{
}

void Light::UseLight(GLint ambIntensityLocation, GLint ambColorLocation, GLint diffuseIntensityLocation)
{
    glUniform3f(ambColorLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(ambIntensityLocation, mAmbientIntensity);

    glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
}

Light::~Light()
{
}
