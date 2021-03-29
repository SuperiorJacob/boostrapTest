#pragma once
#include <list>
#include <vector>
#include <glm/glm.hpp>

class Camera;
class Instance;

const int MAX_LIGHTS = 4;

class Light
{
public:
	Light() 
	{
		m_direction = glm::vec3(1);
		m_color = glm::vec3(1);
		m_untouchedColor = m_color;
		m_intensity = 1;

		SetName("Light");
	}
	Light(glm::vec3 a_pos, glm::vec3 a_color, float a_intensity) 
	{
		m_direction = a_pos;
		m_untouchedColor = a_color;
		m_intensity = a_intensity;
		m_color = a_color * a_intensity;

		SetName("Light");
	}

	void SetName(char* a_name) { strcpy(m_name, a_name); strcpy(m_actualName, a_name); }

	char m_name[32];
	char m_actualName[32];

	glm::vec3 m_direction;
	glm::vec3 m_color;
	glm::vec3 m_untouchedColor;
	float m_intensity;

};

class Scene
{
public:
	Scene(Camera* a_camera, glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight);
	~Scene();

	void AddInstance(Instance* a_instance);
	void Draw();

	void IMGUI_Logic();

	Camera* GetCamera()					{ return m_camera; }
	glm::vec2 GetWindowSize()			{ return m_windowSize; }
	Light& GetLight()					{ return m_light; }
	glm::vec3 GetAmbientLight()			{ return m_ambientLight; }

	int GetNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColors() { return &m_pointLightColors[0]; }

	std::vector<Light>& GetPointLights() { return m_pointLights; }

protected:
	Camera*					m_camera;
	glm::vec2				m_windowSize;
	Light					m_light;
	Light					m_sunLight;
	std::vector<Light>		m_pointLights;
	glm::vec3				m_ambientLight;
	std::list<Instance*>	m_instances;

	glm::vec3				m_pointLightPositions[MAX_LIGHTS];
	glm::vec3				m_pointLightColors[MAX_LIGHTS];
};

