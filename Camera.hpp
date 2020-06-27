#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera(glm::vec3 startCamPos, glm::vec3 startWorldUp, GLfloat srartCamYawDeg, GLfloat startCamPitchDeg, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    ~Camera();

    void KeyControl(const bool *keys, GLfloat deltaTime);
    void MouseContol(GLfloat xChange, GLfloat yChange);

    const glm::vec3 &GetCameraPosition()
    {
        return mCamPos;
    }

    glm::mat4 CalculateViewMatrix();

private:
    glm::vec3 mCamPos;
    glm::vec3 mCamFront;
    glm::vec3 mCamUp;
    glm::vec3 mCamRight;
    glm::vec3 mWorldUp;

    GLfloat mCamYawRad;
    GLfloat mCamPitchRad;

    GLfloat mMoveSpeed;
    GLfloat mTurnSpeedRad;

    glm::mat4 mViewMat{1.0};

    void Update();
};
