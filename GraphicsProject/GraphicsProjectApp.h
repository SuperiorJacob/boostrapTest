#pragma once

#include "Application.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include <glm/mat4x4.hpp>

class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void SolarSystem(float dt);

protected:
	Camera m_camera;

	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture	m_gridTexture;

	// --- SHADER ---
	aie::ShaderProgram	m_simpleShader;
	aie::ShaderProgram	m_bunnyShader;
	aie::ShaderProgram	m_dragonShader;
	aie::ShaderProgram	m_buddhaShader;
	aie::ShaderProgram	m_lucyShader;
	aie::ShaderProgram	m_phongShader;
	aie::ShaderProgram	m_textureShader;
	aie::ShaderProgram	m_normalMapShader;
	// --------------
	// Basic Plane
	Mesh			m_quadMesh;
	glm::mat4		m_quadTransform;

	struct IMGUI_OBJECT_INFO
	{
		std::string name;
		aie::OBJMesh mesh;
		glm::mat4 transform;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	IMGUI_OBJECT_INFO m_bunny;
	IMGUI_OBJECT_INFO m_dragon;
	IMGUI_OBJECT_INFO m_buddha;
	IMGUI_OBJECT_INFO m_lucy;
	IMGUI_OBJECT_INFO m_spear;
	IMGUI_OBJECT_INFO m_ship;

	struct Light
	{
		glm::vec3 direction;
		glm::vec3 color;
	};

	Light m_light;
	glm::vec3 m_ambientLight;

public:
	bool LoadShaderAndMeshLogic();
	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void UpdateObjectTransforms();
	void IMGUI_Logic();
};