#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
	private:
    GLFWwindow * mainWindow;
    GLint width;
    GLint height;
    GLint bufferWidth;
    GLint bufferHeight;

	public:

		Window();
		Window(GLint windowWidth, GLint windowHeight);

    GLint getBufferWidth() {
      return this->bufferWidth;
    }

    GLint getBufferHeight() {
      return this->bufferHeight;
    }

    bool getShouldClose() {
      return glfwWindowShouldClose(this->mainWindow);
    }

    void swapBuffers() {
      glfwSwapBuffers(this->mainWindow);
    }

    int init();
		~Window();

};
#endif
