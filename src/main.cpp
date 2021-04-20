#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint width = 800;
const GLint height = 600;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffest = 0.0f;
float triMaxOffest = 0.7f;
float triIncrement = 0.005f;
const float radians = (M_PI / 180.0f);

static const char * vShader = R"str(
#version 330

layout (location = 0) in vec3 pos;
uniform mat4 model;

void main()
{
  gl_Position = model * vec4(pos, 1.0);
}
)str";

static const char * fShader = R"str(
#version 330

out vec4 colour;

void main()
{
  colour = vec4(1.0, 0.0, 0.0, 1.0);
}
)str";

void createTriangle()
{
  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void addShader(GLuint program, const char * shadeCode, GLenum shaderType)
{
  GLuint createdShader = glCreateShader(shaderType);

  const GLchar * shaderCode[1];
  shaderCode[0] = shadeCode;

  GLint codeLength[1];
  codeLength[0] = strlen(shadeCode);

  glShaderSource(createdShader, 1, shaderCode, codeLength);
  glCompileShader(createdShader);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glGetShaderiv(createdShader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    glGetShaderInfoLog(createdShader, sizeof(eLog), NULL, eLog);
    printf("Error compiling the %d shader: %s\n", shaderType, eLog);
    return;
  }

  glAttachShader(program, createdShader);
}

void compileShaders()
{
  shader = glCreateProgram();

  if (!shader)
  {
    printf("Error creating shader program\n");
    return;
  }

  addShader(shader, vShader, GL_VERTEX_SHADER);
  addShader(shader, fShader, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error linking program: %s\n", eLog);
    return;
  }

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error validating program: %s\n", eLog);
    return;
  }

  uniformModel = glGetUniformLocation(shader, "model");
}

int main(int argc, const char** argv) {
    if (!glfwInit())
    {
      printf("GLFW failed\n");
      glfwTerminate();
      return 1;
    }

    // int major, minor, revision;
    // glfwGetVersion(&major, &minor, &revision);

    // printf("Version %d.%d.%d\n", major, minor, revision);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow * mainWindow = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (!mainWindow) {
      printf("GLFW window creation failed\n");
      glfwTerminate();
      return 1;
    }

    int bufferWidth;
    int bufferHeight;

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
      printf("GLEW init failed");
      glfwDestroyWindow(mainWindow);
      glfwTerminate();
      return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    createTriangle();
    compileShaders();

    while (!glfwWindowShouldClose(mainWindow))
    {
      /* code */
      glfwPollEvents();

      if (direction) {
        triOffest += triIncrement;
      } else {
        triOffest -= triIncrement;

      }

      if (std::abs(triOffest) >= triMaxOffest)
        direction = !direction;

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shader);

      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(triOffest, 0.0f, 0.0f));
      model = glm::rotate(model, 45 * radians, glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
      
      
      glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      glUseProgram(0);

      glfwSwapBuffers(mainWindow);
    }
    
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}