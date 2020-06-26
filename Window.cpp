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

void Window::WindowResized(GLFWwindow *window, int width, int height)
{
    Window *thisPtr = static_cast<Window *>(glfwGetWindowUserPointer(window));
    thisPtr->ResizeWindow(width, height);
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

    glfwSetWindowSizeCallback(mMainWindow, WindowResized);

    // Create Viewport
    glViewport(0, 0, mBufferWidth, mBufferHeight);
}

void Window::ResizeWindow(GLint newWidth, GLint newHeight)
{
    mWidth = newWidth;
    mHeight = newHeight;

    glfwGetFramebufferSize(mMainWindow, &mBufferWidth, &mBufferHeight);

    glViewport(0, 0, mBufferWidth, mBufferHeight);
}
