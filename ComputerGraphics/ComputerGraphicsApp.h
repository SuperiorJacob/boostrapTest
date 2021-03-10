#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include <glm/mat4x4.hpp>

class ComputerGraphicsApp : public aie::Application {
public:

	ComputerGraphicsApp();
	virtual ~ComputerGraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	//void solarSystem(float deltaTime);

protected:

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// ====== SHADER ======
	aie::ShaderProgram m_simpleShader;
	// ====================
	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;

public:
	bool LoadShaderAndMeshLogic();
};