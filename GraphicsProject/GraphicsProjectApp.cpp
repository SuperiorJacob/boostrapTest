#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"

#include "Scene.h"
#include "Instance.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp()
{

}

GraphicsProjectApp::~GraphicsProjectApp()
{

}

bool GraphicsProjectApp::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	Light light;
	light.m_color = { 1, 1, 1 };
	light.m_direction = { 1, -1, -1 };

	return LoadShaderAndMeshLogic(light);
}

void GraphicsProjectApp::shutdown()
{
	Gizmos::destroy();
	delete m_scene;
}

void GraphicsProjectApp::update(float deltaTime)
{
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	m_scene->SetCamera(m_camera);

	m_camera->Update(deltaTime);

	for (int i = 0; i < MAX_LIGHTS && i < m_scene->GetPointLights().size(); i++)
	{
		Light pointLight = m_scene->GetPointLights()[i];
		glm::vec3 pos = pointLight.m_direction;
		glm::vec3 colour = pointLight.m_untouchedColor;

		float radius = pointLight.m_intensity;

		Gizmos::addSphere(pos, radius, 16, 16, glm::vec4(colour.x, colour.y, colour.z, 1));
	}

	IMGUI_Logic();

	//SolarSystem(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->wasKeyPressed(aie::INPUT_KEY_TAB))
	{
		m_cameraID++;

		if (m_cameraID > 3)
		{
			m_cameraID = 0;
		}

		m_camera = m_scene->GetCameras().at(m_cameraID);
	}

}

void GraphicsProjectApp::draw()
{
	if (EnablePostProcessing)
	{
		// Bind the render target
		m_renderTarget.bind();
	}

	// wipe the screen to the background colour
	clearScreen();
	
	glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_camera->GetViewMatrix();

#pragma region Quad
	auto pvm = projectionMatrix * viewMatrix * m_quadTransform;
	m_textureShader.bind();

	m_textureShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the texture to a location of your choice (0)
	m_textureShader.bindUniform("diffuseTexture", 0);

	// Bind the texture to the specificed location
	m_gridTexture.bind(0);

	// Draw the quad...
	m_quadMesh.Draw();

#pragma endregion
	
	m_scene->Draw();

	Gizmos::draw(projectionMatrix * viewMatrix);
	
	if (EnablePostProcessing)
	{
		// Unbind target to return to backbuffer
		m_renderTarget.unbind();

		// Clear the backbuffer
		clearScreen();

		m_postShader.bind();
		m_postShader.bindUniform("ColorTarget", 0);
		m_renderTarget.getTarget(0).bind(0);

		// Fullscreen Quad
		m_fullscreenQuad.Draw();
	}
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic(Light a_light)
{
#pragma region All

#pragma region Quad
	// Load the vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_simpleShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_simpleShader.getLastError());
		return false;
	}

	// Define the 6 vertices for the 2 triangles that make the quad
	//Mesh::Vertex verticesNoIndex[6];
	//verticesNoIndex[0].position = { -0.5f, 0.0f,  0.5f, 1.0f };
	//verticesNoIndex[1].position = { 0.5f, 0.0f,  0.5f, 1.0f };
	//verticesNoIndex[2].position = { -0.5f, 0.0f, -0.5f, 1.0f };

	//verticesNoIndex[3].position = { -0.5f, 0.0f, -0.5f, 1.0f };
	//verticesNoIndex[4].position = { 0.5f, 0.0f,  0.5f, 1.0f };
	//verticesNoIndex[5].position = { 0.5f, 0.0f, -0.5f, 1.0f };

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.0f,  0.5f, 1.0f };
	vertices[1].position = { 0.5f, 0.0f,  0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.0f, -0.5f, 1.0f };
	vertices[3].position = { 0.5f, 0.0f, -0.5f, 1.0f };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	m_quadMesh.InitialiseQuad();
	//m_quadMesh.Initialise(6, vertices, 6, indices);

	// We will make the quad 10 units by 10 units 
	m_quadTransform = {
		10,  0,  0,  0,
		 0, 10,  0,  0,
		 0,  0, 10,  0,
		 0,  0,  0,  1
	};

	/*m_quadTransform = glm::rotate(m_quadTransform, glm::radians(90.0f), glm::vec3(1, 0, 0));*/

#pragma endregion

#pragma region FlatBunny
	if (m_bunny.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Failed!\n");
		return false;
	}

#pragma endregion

#pragma region Dragon
	if (m_dragon.load("./stanford/dragon.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

#pragma endregion

#pragma region Buddha
	m_buddhaShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_buddhaShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_buddhaShader.link())
	{
		printf("Buddha Shader had an error: %s\n", m_buddhaShader.getLastError());
		return false;
	}

	if (m_buddha.load("./stanford/buddha.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

#pragma endregion

#pragma region Lucy
	m_lucyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_lucyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_lucyShader.link())
	{
		printf("Lucy Shader had an error: %s\n", m_lucyShader.getLastError());
		return false;
	}

	if (m_lucy.load("./stanford/lucy.obj") == false)
	{
		printf("Lucy Mesh Failed!\n");
		return false;
	}

	//m_lucy.position = { 6, 0, 0 };
	//m_lucy.scale = { 0.5f, 0.5f, 0.5f };
	//m_lucy.rotation = { 0, 0, 0 };
	//m_lucy.name = "Lucy";

#pragma endregion

#pragma region Phong
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if (m_phongShader.link() == false)
	{
		printf("Phong Shader has an error: %s\n", m_phongShader.getLastError());
		return false;
	}

#pragma endregion

#pragma region TextureShader
	m_textureShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	if (m_textureShader.link() == false)
	{
		printf("Texture Shader had an error: %s\n", m_textureShader.getLastError());
		return false;
	}
#pragma endregion
	
#pragma region NormalMapShader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

	if (m_normalMapShader.link() == false)
	{
		printf("Normal Map Shader has an error: %s\n", m_normalMapShader.getLastError());
		return false;
	}

#pragma endregion

#pragma region Post Proccessor
	if (m_renderTarget.initialise(1, (float)getWindowWidth(), (float)getWindowHeight()) == false)
	{
		printf("Render Target Error!\n");
		return false;
	}

	// Create a fullscreen quad
	m_fullscreenQuad.InitialiseFullscreenQuad();

	// Load a processing shader
	m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/post.frag");

	if (m_postShader.link() == false)
	{
		printf("Post Shader has an error: %s\n", m_postShader.getLastError());
		return false;
	}
#pragma endregion

#pragma region GridLogic
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load: numbered_grid.tga\n");
		return false;
	}
#pragma endregion

#pragma region Soulspear
	if (m_spear.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh has had an error!\n");
		return false;
	}

#pragma endregion

#pragma region Barrel
	if (m_barrel.load("./taru/taru.obj", true, false) == false)
	{
		printf("Barrel Mesh has had an error!\n");
		return false;
	}

#pragma endregion

#pragma endregion

	m_scene = new Scene(m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));

	// Yes horrific
	Instance* inst = new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), &m_spear, &m_normalMapShader);
	inst->SetName("Spear - Normal");
	m_scene->AddInstance(inst);

	Instance* inst1 = new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), &m_barrel, &m_normalMapShader);
	inst1->SetName("Barrel - Normal");
	m_scene->AddInstance(inst1);

	Instance* inst2 = new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.5f), &m_dragon, &m_phongShader);
	inst2->SetName("Dragon - Phong");
	m_scene->AddInstance(inst2);
	//

	// Add a red light on the left side
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));

	// Add a green light on the right side
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));

#pragma region Cameras
	// Flyby Cam
	Camera* m_flybyCam = new Camera(glm::vec3(-10, 2, 0), 0.0f, 0.0f, true);
	m_scene->AddCamera(m_flybyCam);

	// X-axis Cam
	Camera* m_xAxisCam = new Camera(glm::vec3(-20, 4, 0), 0.0f, 0.0f, false);
	m_scene->AddCamera(m_xAxisCam);

	// Y-axis Cam
	Camera* m_yAxisCam = new Camera(glm::vec3(0, 20, 0), 0.0f, -90.0f, false);
	m_scene->AddCamera(m_yAxisCam);

	// Z-axis Cam
	Camera* m_zAxisCam = new Camera(glm::vec3(0, 4, 20), -90.0f, 0.0f, false);
	m_scene->AddCamera(m_zAxisCam);

	// Default cam is flyby cam
	m_camera = m_flybyCam;

#pragma endregion Cameras

	return true;
}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::ColorEdit3("Sunlight Color", &m_scene->GetLight().m_color[0]);

	ImGui::Checkbox("Enable Distort Post-processing", &EnablePostProcessing);

	ImGui::End();

	m_scene->IMGUI_Logic();
}
