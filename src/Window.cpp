#include "Window.hpp"
#include <spdlog/spdlog.h>

Window::Window()
{
	this->width = 800;
	this->height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
  this->width = windowWidth;
  this->height = windowHeight;
}

int Window::init()
{
  if (!glfwInit())
    {
      spdlog::error("GLFW failed");
      glfwTerminate();
      return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    this->mainWindow = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (!mainWindow) {
      spdlog::error("GLFW window creation failed");
      glfwTerminate();
      return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
      spdlog::error("GLEW init failed");
      glfwDestroyWindow(mainWindow);
      glfwTerminate();
      return 1;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);
    return 0;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
  glfwTerminate();
}
