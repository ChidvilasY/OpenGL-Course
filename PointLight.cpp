#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "PointLight.hpp"

PointLight::PointLight(GLsizei shadowWidth, GLsizei shadowHeight,
                       GLfloat near, GLfloat far,
                       GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat ambIntensity, GLfloat diffuseIntensity,
                       GLfloat xPos, GLfloat yPos, GLfloat zPos,
                       GLfloat con, GLfloat lin, GLfloat quad)
    : Light(shadowWidth, shadowHeight, red, green, blue, ambIntensity, diffuseIntensity),
      mPosition(glm::vec3(xPos, yPos, zPos)),
      mConstant(con), mLinear(lin), mQuadratic(quad),
      mFarPlane(far)
{
    float aspect = static_cast<float>(shadowWidth) / static_cast<float>(shadowHeight);
    mLightProj = glm::perspective(glm::radians(90.f), aspect, near, far);

    mShadowMap = new OmniShadowMap();
    mShadowMap->Init(shadowWidth, shadowHeight);
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

std::array<glm::mat4, 6> PointLight::CalculateLightTransform()
{
    std::array<glm::mat4, 6> lightTransforms;

    // Up vector is -1y because of Normals convention followed in this tutorial
    // +x -x
    lightTransforms[0] = mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
    lightTransforms[1] = mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f));

    // +y -y
    lightTransforms[2] = mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
    lightTransforms[3] = mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f));

    // +z -z
    lightTransforms[4] = mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f));
    lightTransforms[5] = mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f));

    return lightTransforms;
}