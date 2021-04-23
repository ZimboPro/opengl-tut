#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Mesh  
{
	private:
		GLuint VAO, VBO, IBO;
		GLsizei indexCount;

	public:

		Mesh();
		~Mesh();

		void createMesh(GLfloat *vertices, unsigned int * indices, unsigned int noOfVertices, unsigned int numOfIndices);
		void renderMesh();
		void clearMesh();

};
#endif