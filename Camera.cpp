#include "Camera.hpp"

Camera::Camera(glm::vec3 startCamPos, glm::vec3 startWorldUp, GLfloat srartCamYawDeg,
               GLfloat startCamPitchDeg, GLfloat startMoveSpeed, GLfloat startTurnSpeedDeg)
    : mCamPos(startCamPos),
      mCamFront(glm::vec3(0.f, 0.f, -1.f)),
      mCamUp(glm::vec3(0.f, 1.f, 0.f)),
      mWorldUp(startWorldUp),
      mCamYawRad(glm::radians(srartCamYawDeg)),
      mCamPitchRad(glm::radians(startCamPitchDeg)),
      mMoveSpeed(startMoveSpeed),
      mTurnSpeedRad(glm::radians(startTurnSpeedDeg))
{
    Update();
}

Camera::~Camera()
{
}

void Camera::Update()
{
    mCamFront.x = cosf(mCamYawRad) * cosf(mCamPitchRad);
    mCamFront.y = sinf(mCamPitchRad);
    mCamFront.z = sinf(mCamYawRad) * cosf(mCamPitchRad);
    mCamFront = glm::normalize(mCamFront);

    mCamRight = glm::normalize(glm::cross(mCamFront, mWorldUp));
    mCamUp = glm::normalize(glm::cross(mCamRight, mCamFront));
}

void Camera::KeyControl(const bool *keys, GLfloat deltaTime)
{
    GLfloat velocity = mMoveSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        mCamPos += mCamFront * velocity;
    }

    if (keys[GLFW_KEY_S])
    {
        mCamPos -= mCamFront * velocity;
    }

    if (keys[GLFW_KEY_A])
    {
        mCamPos -= mCamRight * velocity;
    }

    if (keys[GLFW_KEY_D])
    {
        mCamPos += mCamRight * velocity;
    }

    Update();
}

void Camera::MouseContol(GLfloat xChange, GLfloat yChange)
{
    xChange *= mTurnSpeedRad;
    yChange *= mTurnSpeedRad;

    mCamYawRad += xChange;
    mCamPitchRad += yChange;

    if (mCamPitchRad > glm::pi<GLfloat>() / 2.f)
    {
        mCamPitchRad = glm::pi<GLfloat>() / 2.f;
    }
    else if (mCamPitchRad < glm::pi<GLfloat>() / -2.f)
    {
        mCamPitchRad = glm::pi<GLfloat>() / -2.f;
    }

    Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
    mViewMat = glm::lookAt(mCamPos, mCamPos + mCamFront, mCamUp);

    return mViewMat;
}
