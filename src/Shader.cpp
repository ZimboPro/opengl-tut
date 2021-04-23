#include "Shader.hpp"
#include <spdlog/spdlog.h>

Shader::Shader()
{
	this->shaderId = 0;
  this->uniformModel = 0;
  this->uniformProjection = 0;
}

Shader::~Shader()
{
	this->clearShader();
}

void Shader::createFromString(const char * vertexCode, const char * fragmentCode)
{
  this->compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char * vertexFile, const char * fragmentFile)
{
  std::string vertexCode = this->readFile(vertexFile);
  std::string fragmentCode = this->readFile(fragmentFile);
  this->createFromString(vertexCode.c_str(), fragmentCode.c_str());
}

GLuint Shader::getProjectionLocation()
{
  return this->uniformProjection;
}

GLuint Shader::getModelLocation()
{
  return this->uniformModel;
}

void Shader::useShader()
{
  glUseProgram(this->shaderId);
}

void Shader::clearShader()
{
  if (this->shaderId != 0) {
    glDeleteProgram(this->shaderId);
    this->shaderId = 0;
  }
}

void Shader::compileShader(const char * vertexCode, const char * fragmentCode)
{

  shaderId = glCreateProgram();

  if (!shaderId)
  {
    spdlog::error("Error creating shader program");
    return;
  }

  addShader(shaderId, vertexCode, GL_VERTEX_SHADER);
  addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glLinkProgram(shaderId);
  glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
    spdlog::error("Error linking program: %s", eLog);
    return;
  }

  glValidateProgram(shaderId);
  glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
    spdlog::error("Error validating program: %s", eLog);
    return;
  }

  this->uniformModel = glGetUniformLocation(shaderId, "model");
  uniformProjection = glGetUniformLocation(shaderId, "projection");
}

void Shader::addShader(GLuint program, const char * shadeCode, GLenum shaderType)
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
    spdlog::error("Error compiling the %d shader: %s", shaderType, eLog);
    return;
  }

  glAttachShader(program, createdShader);
}

std::string Shader::readFile(const char * path)
{
  std::string content;
  std::ifstream fileStream(path, std::ios::in);

  if (!fileStream.is_open()) {
    spdlog::error("Failed to read %s! File does not exist", path);
    return "";
  }

  std::string line = "";
  while (!fileStream.eof())
  {
    std::getline(fileStream, line);
    content.append(line + '\n');
  }

  fileStream.close();
  return content;
}
