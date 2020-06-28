#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat ambIntensity, GLfloat diffuseIntensity,
                                   GLfloat xDir, GLfloat yDir, GLfloat zDir)
    : Light(red, green, blue, ambIntensity, diffuseIntensity),
      mDirection(glm::vec3(xDir, yDir, zDir))
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

DirectionalLight::~DirectionalLight()
{
}
