#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
	private:
		GLuint shaderId;
		GLuint uniformProjection;
		GLuint uniformModel;

		void compileShader(const char * vertexCode, const char * fragmentCode);
		void addShader(GLuint program, const char * shadeCode, GLenum shaderType);

    std::string readFile(const char * path);

	public:

		Shader();
		~Shader();

		void createFromString(const char * vertexCode, const char * fragmentCode);
		void createFromFiles(const char * vertexFile, const char * fragmentFile);

		GLuint getProjectionLocation();
		GLuint getModelLocation();

		void useShader();
		void clearShader();

};
#endif
