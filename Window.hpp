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

    static void WindowResized(GLFWwindow* window, int width, int height);
    void ResizeWindow(GLint newWidth, GLint newHeight);

    ~Window();

private:
    GLFWwindow *mMainWindow;

    GLint mWidth, mHeight;
    GLint mBufferWidth, mBufferHeight;
};
