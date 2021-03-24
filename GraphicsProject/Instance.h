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

	char* m_name = "Game Object";
	glm::vec3				m_rotation;

protected:
	glm::mat4				m_transform;
	aie::OBJMesh*			m_mesh;
	aie::ShaderProgram*		m_shader;
};

