#pragma once

#include "Application.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

#include "Scene.h"
#include "Instance.h"

#include "OBJMesh.h"
#include <glm/mat4x4.hpp>
#include <vector>

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

	Scene* m_scene;

	// --- SHADER ---
	//// Shadows
	//aie::ShaderProgram	m_useShadowProgram;
	////
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

	aie::OBJMesh m_bunny;
	aie::OBJMesh m_dragon;
	aie::OBJMesh m_buddha;
	aie::OBJMesh m_lucy;
	aie::OBJMesh m_spear;
	aie::OBJMesh m_barrel;

	//struct Light
	//{
	//	glm::vec3 direction;
	//	glm::vec3 color;
	//};

	//Light m_light;
	//glm::vec3 m_ambientLight;

public:
	bool LoadShaderAndMeshLogic(Light a_light);
	//void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void IMGUI_Logic();
};