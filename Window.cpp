#include <string>
#include <stdexcept>

#include "Window.hpp"

Window::Window(GLint windowWidth, GLint windowHeight)
    : mMainWindow(nullptr),
      mWidth(windowWidth),
      mHeight(windowHeight)
{
}

Window::~Window()
{
    glfwDestroyWindow(mMainWindow);
    glfwTerminate();
}

void Window::Initialize()
{
    if (!glfwInit())
    {
        glfwTerminate();
        throw std::runtime_error("Error Initialising GLFW");
    }

    // Setup GLFW Windows Properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatiblity
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    mMainWindow = glfwCreateWindow(mWidth, mHeight, "Test Window", NULL, NULL);
    if (!mMainWindow)
    {
        glfwTerminate();
        throw std::runtime_error("Error creating GLFW window!");
    }

    // Get buffer size information
    glfwGetFramebufferSize(mMainWindow, &mBufferWidth, &mBufferHeight);

    // Set the current context
    glfwMakeContextCurrent(mMainWindow);

    // Allow modern extension access
    glewExperimental = GL_TRUE;

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        throw std::runtime_error("Error: " + std::string(reinterpret_cast<const char *>(glewGetErrorString(error))));
        glfwDestroyWindow(mMainWindow);
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(mMainWindow, this);
    // glfwSetInputMode(mMainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Create Viewport
    glViewport(0, 0, mBufferWidth, mBufferHeight);

    CreateCallBacks();
}

void Window::CreateCallBacks()
{
    glfwSetWindowSizeCallback(mMainWindow, WindowResized);

    glfwSetKeyCallback(mMainWindow, HandleKeys);

    glfwSetCursorPosCallback(mMainWindow, HandleCursor);
}

void Window::WindowResized(GLFWwindow *window, int newWidth, int newHeight)
{
    Window *thisPtr = static_cast<Window *>(glfwGetWindowUserPointer(window));

    thisPtr->mWidth = newWidth;
    thisPtr->mHeight = newHeight;

    glfwGetFramebufferSize(thisPtr->mMainWindow, &thisPtr->mBufferWidth, &thisPtr->mBufferHeight);

    glViewport(0, 0, thisPtr->mBufferWidth, thisPtr->mBufferHeight);
}

void Window::HandleKeys(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Window *thisPtr = static_cast<Window *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            thisPtr->mKeysPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            thisPtr->mKeysPressed[key] = false;
        }
    }
}

void Window::HandleCursor(GLFWwindow *window, double xPosDouble, double yPosDouble)
{
    GLfloat xPos = static_cast<GLfloat>(xPosDouble);
    GLfloat yPos = static_cast<GLfloat>(yPosDouble);

    Window *thisPtr = static_cast<Window *>(glfwGetWindowUserPointer(window));

    if (thisPtr->mouseFirstMoved)
    {
        thisPtr->mMouseLastX = xPos;
        thisPtr->mMouseLastY = yPos;
        thisPtr->mouseFirstMoved = false;
    }

    thisPtr->mMouseChangeX = xPos - thisPtr->mMouseLastX ;
    thisPtr->mMouseChangeY = thisPtr->mMouseLastY - yPos;

    thisPtr->mMouseLastX = xPos;
    thisPtr->mMouseLastY = yPos;
}
