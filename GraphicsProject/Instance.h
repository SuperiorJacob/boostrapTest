/*

* File:			Instance.h
* Author:		Jacob Cooper (s200503@students.aie.edu.au)

* Edit Dates:
*	First:		18/03/2021
*	Last:		8/04/2021

* Summary:
*	This class is built to store, draw and manipulate object data; eg. mesh, transform, shaders, name, etc.
*/

#pragma once
#include <glm/glm.hpp>

class Camera;
class Scene;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{

public:
	Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);
	Instance(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale, aie::OBJMesh* a_mesh,
		aie::ShaderProgram* a_shader);
	void Draw(Scene* a_scene);

	glm::mat4 GetTransform() { return m_transform; }
	void SetTransform(glm::mat4 a_transform) { m_transform = a_transform; }
	static glm::mat4 MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale);

	void SetName(char* a_name) { strcpy(m_name, a_name); strcpy(m_actualName, a_name); }

	char m_name[32];
	char m_actualName[32];
	glm::vec3				m_rotation;

protected:
	glm::mat4				m_transform;
	aie::OBJMesh*			m_mesh;
	aie::ShaderProgram*		m_shader;
};

