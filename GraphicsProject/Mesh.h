/*

* File:			Mesh.h
* Author:		Jacob Cooper (s200503@students.aie.edu.au)

* Edit Dates:
*	First:		12/03/2021
*	Last:		8/04/2021

*/

#pragma once
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {};
	virtual ~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	void InitialiseQuad();
	void Initialise(unsigned int a_vertexCount, const Vertex* a_vertices, unsigned int a_indexCount = 0, unsigned int* a_indices = nullptr);
	void InitialiseFullscreenQuad();

	virtual void Draw();

protected:
	unsigned int	triCount;
	unsigned int	vao, 	// Vertex Array Object
					vbo, 	// Vertex Buffer Object
					ibo; 	// Index Buffer Object

};