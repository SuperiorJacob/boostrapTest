/*

* File:			GraphicsProjectApp.h
* Author:		Jacob Cooper (s200503@students.aie.edu.au)

* Edit Dates:
*	First:		12/03/2021
*	Last:		8/04/2021

* Summary:
*	Main bootstrap graphics project application, used to run and process the entire game whilst also
*	storing core member variables used throughout the project like mesh, rendertarget, shader and camera data.

*/

#pragma once

#include "Application.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderTarget.h"

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

	bool EnablePostProcessing = false;

protected:
	Camera* m_camera;
	int m_cameraID = 0;

	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture	m_gridTexture;

	Scene* m_scene;

	// --- SHADER ---
	aie::ShaderProgram	m_simpleShader;
	aie::ShaderProgram	m_bunnyShader;
	aie::ShaderProgram	m_dragonShader;
	aie::ShaderProgram	m_buddhaShader;
	aie::ShaderProgram	m_lucyShader;
	aie::ShaderProgram	m_phongShader;
	aie::ShaderProgram	m_textureShader;
	aie::ShaderProgram	m_normalMapShader;
	aie::ShaderProgram	m_postShader;
	// --------------
	// Basic Plane
	Mesh			m_quadMesh;
	glm::mat4		m_quadTransform;

	Mesh m_fullscreenQuad;

	aie::OBJMesh m_bunny;
	aie::OBJMesh m_dragon;
	aie::OBJMesh m_buddha;
	aie::OBJMesh m_lucy;
	aie::OBJMesh m_spear;
	aie::OBJMesh m_barrel;

	aie::RenderTarget m_renderTarget;

public:
	bool LoadShaderAndMeshLogic(Light a_light);
	//void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void IMGUI_Logic();
};