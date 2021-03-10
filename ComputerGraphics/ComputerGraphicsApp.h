#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class ComputerGraphicsApp : public aie::Application {
public:

	ComputerGraphicsApp();
	virtual ~ComputerGraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void solarSystem(float deltaTime);
	glm::vec3 createPlanet(glm::vec3 pos, glm::vec4 dist, float size, float angleRad, glm::vec3 rotAxis, glm::vec4 color);

protected:

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	//float m_upTime = 0.0f;
};