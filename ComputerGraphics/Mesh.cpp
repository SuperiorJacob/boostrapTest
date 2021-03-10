#include <gl_core_4_4.h>
#include "Mesh.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::InitialiseQuad()
{
	// Check that the mesh has not been initialised already.
	assert(vao == 0);

	// Generate the buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Binds the vertex array; a mesh wrapper
	glBindVertexArray(vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Define the 6 vertices for the 2 triangles that make the quad
	Vertex vertices[6];
	vertices[0].position = { -0.5,	0.f,	0.5f,	1.f };
	vertices[1].position = { 0.5,	0.f,	0.5f,	1.f };
	vertices[2].position = { -0.5,	0.f,	-0.5f,	1.f };

	vertices[3].position = { -0.5,	0.f,	-0.5f,	1.f };
	vertices[4].position = { 0.5,	0.f,	0.5f,	1.f };
	vertices[5].position = { 0.5,	0.f,	-0.5f,	1.f };

	// Fill the vertex buffer 
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Enable the first element as a position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Quad has two triangles
	triCount = 2;
}

void Mesh::Draw()
{
	glBindVertexArray(vao);

	// Check if we are using indices or just vertices
	if (ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNDEFINED_VERTEX, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
}
