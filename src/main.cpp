#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Window.hpp"

const GLint width = 800;
const GLint height = 600;

std::vector<Mesh *> meshList;
std::vector<Shader *> shaderList;

static const char * vShader = "Shaders/shaderVert.glsl";

static const char * fShader = "Shaders/shaderFrag.glsl";

void createTriangle()
{
  unsigned int indices[] {
    0,3,1,
    1,3,2,
    2,3,0,
    0,1,2
  };

  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };

  Mesh *obj = new Mesh();
  obj->createMesh(vertices, indices, 12, 12);
  meshList.push_back(obj);
}

void createShaders() {
  Shader * shader1 = new Shader();
  shader1->createFromFiles(vShader, fShader);
  shaderList.push_back(shader1);
}

int main(int argc, const char** argv) {
    spdlog::info("start program");
    Window mainWidow(width, height);
    mainWidow.init();

    createTriangle();
    createShaders();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
    ((GLfloat)mainWidow.getBufferWidth()) / ((GLfloat)mainWidow.getBufferHeight()), 0.1f, 100.0f);
    GLuint uniformModel, uniformProjection;

    while (!mainWidow.getShouldClose())
    {
      glfwPollEvents();

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shaderList[0]->useShader();

      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
      model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

      uniformModel = shaderList[0]->getModelLocation();
      uniformProjection = shaderList[0]->getProjectionLocation();
      glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

      for (size_t i = 0; i < meshList.size(); i++)
      {
        meshList[i]->renderMesh();
      }

      glUseProgram(0);

      mainWidow.swapBuffers();
    }

    for (size_t i = 0; i < meshList.size(); i++)
    {
      delete meshList[i];
    }
    return 0;
}
