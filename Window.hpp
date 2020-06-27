#pragma once

#include <utility>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{

public:
    Window(GLint windowWidth = 1280, GLint windowHeight = 720);

    void Initialize();

    std::pair<GLint, GLint> GetBufferDim()
    {
        return std::make_pair(mWidth, mHeight);
    }

    bool ShouldWindowClose()
    {
        return glfwWindowShouldClose(mMainWindow);
    }

    void SwapBuffers()
    {
        glfwSwapBuffers(mMainWindow);
    }

    const bool* GetKeyStates()
    {
        return mKeysPressed;
    }

    GLfloat GetCursorChangeX()
    {
        GLfloat mouseChagneX = mMouseChangeX;
        mMouseChangeX = 0.f;
        return mouseChagneX;
    }

    GLfloat GetCursorChangeY()
    {
        GLfloat mouseChangeY = mMouseChangeY;
        mMouseChangeY = 0.f;
        return mouseChangeY;
    }

    ~Window();

private:
    GLFWwindow *mMainWindow;

    GLint mWidth, mHeight;
    GLint mBufferWidth, mBufferHeight;

    // Key States
    bool mKeysPressed[1024]{};

    // Cursor
    GLfloat mMouseLastX;
    GLfloat mMouseLastY;
    GLfloat mMouseChangeX = 0;
    GLfloat mMouseChangeY = 0;
    bool mouseFirstMoved = true;

    // Callbacks
    void CreateCallBacks();
    static void HandleKeys(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void HandleCursor(GLFWwindow *window, double xpos, double ypos);
    static void WindowResized(GLFWwindow *window, int width, int height);
};
