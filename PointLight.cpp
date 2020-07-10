#include "PointLight.hpp"

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat ambIntensity, GLfloat diffuseIntensity,
                       GLfloat xPos, GLfloat yPos, GLfloat zPos,
                       GLfloat con, GLfloat lin, GLfloat quad)
    : Light(1024, 1024, red, green, blue, ambIntensity, diffuseIntensity),
      mPosition(glm::vec3(xPos, yPos, zPos)),
      mConstant(con), mLinear(lin), mQuadratic(quad)
{
}

PointLight::~PointLight()
{
}

void PointLight::UseLight(GLint ambIntensityLocation, GLint colorLocation,
                          GLint diffuseIntensityLocation, GLint positionLocation,
                          GLint constantLocation, GLint linearLocation, GLint quadraticLocation)
{
  glUniform3f(colorLocation, mColor.x, mColor.y, mColor.z);
  glUniform1f(ambIntensityLocation, mAmbientIntensity);
  glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);

  glUniform3f(positionLocation, mPosition.x, mPosition.y, mPosition.z);
  glUniform1f(constantLocation, mConstant);
  glUniform1f(linearLocation, mLinear);
  glUniform1f(quadraticLocation, mQuadratic);
}