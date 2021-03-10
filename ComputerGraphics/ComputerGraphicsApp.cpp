#include "ComputerGraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"

#define GLM_ENABLE_EXPERIMENTAL 1;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

ComputerGraphicsApp::ComputerGraphicsApp() {

}

ComputerGraphicsApp::~ComputerGraphicsApp() {

}

bool ComputerGraphicsApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	return LoadShaderAndMeshLogic();
}

void ComputerGraphicsApp::shutdown() {

	Gizmos::destroy();
}

void ComputerGraphicsApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	solarSystem(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void ComputerGraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

bool ComputerGraphicsApp::LoadShaderAndMeshLogic()
{
	// Load the vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
	if (!m_simpleShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_simpleShader.getLastError());
		return false;
	}

	m_quadMesh.InitialiseQuad();

	// We will make the quad 10 units by 10 units
	m_quadTransform = {
		1,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,1
	};

	return true;
}

//void solarSystem(float deltaTime)
//{
//	std::cout << deltaTime << " " << m_upTime << std::endl;
//	m_upTime += deltaTime;
//	
//	vec3 rotAxis(0.0f, 1.0f, 0.0f);
//
//	float angleRad = glm::half_pi<float>() * m_upTime;
//
//	vec3 sun = createPlanet(vec3(0), vec4(0), 54, angleRad, rotAxis, vec4(1, 1, 0, 1));
//	vec3 mercury = createPlanet(sun, vec4(58, 0, 0, 0), 0.4, angleRad * 4, rotAxis, vec4(0.8f, 0.8f, 0.8f, 1));
//	vec3 vensus = createPlanet(sun, vec4(60, 0, 0, 0), 1, angleRad * 1.6, rotAxis, vec4(1.f, 0.27f, 0.f, 1));
//	vec3 earth = createPlanet(sun, vec4(64, 0, 0, 0), 1, angleRad, rotAxis, vec4(0.f, 0.f, 0.5f, 1));
//	vec3 mars = createPlanet(sun, vec4(66, 0, 0, 0), 0.5, angleRad * 0.05, rotAxis, vec4(1.f, 0.5f, 0.2f, 1));
//	vec3 jupitar = createPlanet(sun, vec4(80, 0, 0, 0), 11, angleRad * 0.03, rotAxis, vec4(0.8f, 0.5f, 0.f, 1));
//	vec3 saturn = createPlanet(sun, vec4(102, 0, 0, 0), 9.5, angleRad * 0.012, rotAxis, vec4(1.f, 1.f, 0.8f, 1));
//	vec3 uranus = createPlanet(sun, vec4(122, 0, 0, 0), 2.3, angleRad * 0.43, rotAxis, vec4(0.2f, 0.2f, 0.8f, 1));
//	vec3 neptune = createPlanet(sun, vec4(126, 0, 0, 0), 2.5, angleRad * 0.22, rotAxis, vec4(0.2f, 0.2f, 1.f, 1));
//
//	//vec3 dad = createPlanet(sun, vec4(6, 0, 0, 0), 1, angleRad, rotAxis, vec4(1, 0, 0, 1));
//	//Gizmos::addSphere(sun, 2, 16, 16, vec4(1, 1, 0, 1));
//	//Gizmos::addSphere(mercury, 1, 16, 16, vec4(1, 0, 0, 1));
//}
//
//vec3 createPlanet(vec3 pos, vec4 dist, float size, float angleRad, vec3 rotAxis, vec4 color)
//{
//	vec3 p = glm::rotate(angleRad, rotAxis) * dist;
//	Gizmos::addSphere(pos + p, size, 16, 16, color);
//
//	return pos + p;
//}
