#include "SpotLight.hpp"

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat ambIntensity, GLfloat diffuseIntensity,
                     GLfloat xPos, GLfloat yPos, GLfloat zPos,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir,
                     GLfloat con, GLfloat lin, GLfloat quad,
                     GLfloat edge)
    : PointLight(red, green, blue, ambIntensity, diffuseIntensity,
                 xPos, yPos, zPos, con, lin, quad),
      mDirection(glm::vec3(xDir, yDir, zDir)),
      mEdge(edge),
      mProcessedEdge(cosf(glm::radians(edge))) {}

SpotLight::~SpotLight() {}

void SpotLight::UseLight(GLint ambIntensityLocation, GLint colorLocation,
                         GLint diffuseIntensityLocation, GLint positionLocation, GLint directionLocation,
                         GLint constantLocation, GLint linearLocation, GLint quadraticLocation,
                         GLint edgeLocation)
{
    glUniform3f(colorLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(ambIntensityLocation, mAmbientIntensity);
    glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
    glUniform3f(directionLocation, mDirection.x, mDirection.y, mDirection.z);

    glUniform3f(positionLocation, mPosition.x, mPosition.y, mPosition.z);
    glUniform1f(constantLocation, mConstant);
    glUniform1f(linearLocation, mLinear);
    glUniform1f(quadraticLocation, mQuadratic);

    glUniform1f(edgeLocation, mProcessedEdge);
}

void SpotLight::SetFlash(const glm::vec3 &pos, const glm::vec3 &dir)
{
    mPosition = pos;
    mDirection = dir;
}
