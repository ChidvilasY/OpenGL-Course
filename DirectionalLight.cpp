#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
                                   GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat ambIntensity, GLfloat diffuseIntensity,
                                   GLfloat xDir, GLfloat yDir, GLfloat zDir)
    : Light(shadowWidth, shadowHeight, red, green, blue, ambIntensity, diffuseIntensity),
      mDirection(glm::vec3(xDir, yDir, zDir))
{
    mLightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
    mShadowMap->Init(shadowWidth, shadowHeight);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UseLight(GLint ambIntensityLocation, GLint ambColorLocation,
                                GLint diffuseIntensityLocation, GLint diffuseDirection)
{
    glUniform3f(ambColorLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(ambIntensityLocation, mAmbientIntensity);

    glUniform3f(diffuseDirection, mDirection.x, mDirection.y, mDirection.z);
    glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
    return mLightProj * glm::lookAt(-mDirection, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}