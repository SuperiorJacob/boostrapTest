#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"

#define GLM_ENABLE_EXPERIMENTAL 1

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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

	m_light.color = { 1, 1, 1 };
	m_ambientLight = { 1.25f, 1.25f, 1.25f};

	return LoadShaderAndMeshLogic();
}

void GraphicsProjectApp::shutdown()
{
	Gizmos::destroy();
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

	m_camera.Update(deltaTime);

	IMGUI_Logic();

	float time = getTime();

	m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	//SolarSystem(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	
	glm::mat4 projectionMatrix = m_camera.GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_camera.GetViewMatrix();

	// update perspective based on screen size
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	DrawShaderAndMeshes(projectionMatrix, viewMatrix);

	Gizmos::draw(projectionMatrix * viewMatrix);
}

void GraphicsProjectApp::SolarSystem(float dt)
{
	// Sun
	Gizmos::addSphere(glm::vec3(0, 0, 0), 1.0f, 16, 16, glm::vec4(1, 0.8, 0, 1));

	//Mercury
	/*glm::vec3 mercuryPos = glm::rotate(10.0f, glm::vec3(2, 0, 0));*/
	Gizmos::addSphere(glm::vec3(2, 0, 0), 0.15f, 16, 16, glm::vec4(1, 0.8, 0, 1));
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic()
{
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
	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_bunnyShader.link())
	{
		printf("Bunny Shader had an error: %s\n", m_bunnyShader.getLastError());
		return false;
	}

	if (m_bunny.mesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Failed!\n");
		return false;
	}

	m_bunny.position = { -8, 0, 0 };
	m_bunny.scale = { 0.5f, 0.5f, 0.5f };
	m_bunny.rotation = { 0, 0, 0 };
	m_bunny.name = "Bunny";

#pragma endregion

#pragma region Dragon
	m_dragonShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_dragonShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_dragonShader.link())
	{
		printf("Dragon Shader had an error: %s\n", m_dragonShader.getLastError());
		return false;
	}

	if (m_dragon.mesh.load("./stanford/dragon.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

	m_dragon.position = { -3, 0, 0 };
	m_dragon.scale = { 0.5f, 0.5f, 0.5f };
	m_dragon.rotation = { 0, 0, 0 };
	m_dragon.name = "Dragon";

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

	if (m_buddha.mesh.load("./stanford/buddha.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

	m_buddha.position = { 3, 0, 0 };
	m_buddha.scale = { 0.5f, 0.5f, 0.5f };
	m_buddha.rotation = { 0, 0, 0 };
	m_buddha.name = "Buddha";

#pragma endregion

#pragma region Lucy
	m_lucyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_lucyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_lucyShader.link())
	{
		printf("Buddha Shader had an error: %s\n", m_lucyShader.getLastError());
		return false;
	}

	if (m_lucy.mesh.load("./stanford/lucy.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

	m_lucy.position = { 6, 0, 0 };
	m_lucy.scale = { 0.5f, 0.5f, 0.5f };
	m_lucy.rotation = { 0, 0, 0 };
	m_lucy.name = "Lucy";

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

#pragma region GridLogic
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load: numbered_grid.tga\n");
		return false;
	}
#pragma endregion

#pragma region Soulspear
	if (m_spear.mesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh has had an error!\n");
		return false;
	}

	m_spear.position = { -3, 0, 0 };
	m_spear.scale = { 0.5f, 0.5f, 0.5f };
	m_spear.rotation = { 0, 0, 0 };
	m_spear.name = "Soul Spear";

#pragma endregion

#pragma region Ship
	if (m_ship.mesh.load("./trifighter/Tri_Fighter.obj", true, false) == false)
	{
		printf("Ship Mesh has had an error!\n");
		return false;
	}

	m_ship.position = { -3, 0, 0 };
	m_ship.scale = { 0.01f, 0.01f, 0.01f };
	m_ship.rotation = { 0, 0, 0 };
	m_ship.name = "Ship";

#pragma endregion

	return true;
}

void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
{
	// PVM = Projection View Matrix
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);

#pragma region Quad
	// Bind the shader
	m_textureShader.bind();

	// Bind the transform of the mesh
	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform;
	m_textureShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the texture to a location of your choice (0)
	m_textureShader.bindUniform("diffuseTexture", 0);

	// Bind the texture to the specificed location
	m_gridTexture.bind(0);

	// Draw the quad...
	m_quadMesh.Draw();

#pragma endregion

#pragma region FlatBunny
	m_bunnyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_bunny.transform;
	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
	m_bunnyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	// Draw bunny mesh
	// m_bunny.mesh.draw();

#pragma endregion

#pragma region Dragon
	m_dragonShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_dragon.transform;
	m_dragonShader.bindUniform("ProjectionViewModel", pvm);
	m_dragonShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	// Draw dragon mesh
	// m_dragon.mesh.draw();

#pragma endregion

#pragma region Buddha
	m_buddhaShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_buddha.transform;
	m_buddhaShader.bindUniform("ProjectionViewModel", pvm);
	m_buddhaShader.bindUniform("MeshFlatColor", glm::vec4(1, 0, 1, 1));

	// Draw buddha mesh
	// m_buddha.mesh.draw();

#pragma endregion

#pragma region Lucy
	m_lucyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_lucy.transform;
	m_lucyShader.bindUniform("ProjectionViewModel", pvm);
	m_lucyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 1, 1));

	// Draw lucy mesh
	// m_lucy.mesh.draw();

#pragma endregion

#pragma region Phong
	// Bind the shader
	m_phongShader.bind();

	// Bind the camera position
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));

	// Bind the light
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	m_phongShader.bindUniform("LightColor", m_light.color);
	m_phongShader.bindUniform("LightDirection", m_light.direction);

	// --- BUNNY ---
	// Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_bunny.transform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_bunny.transform);

	// Draw mesh
	m_bunny.mesh.draw();

	// --- DRAGON ---
	// Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_dragon.transform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_dragon.transform);

	// Draw mesh
	m_dragon.mesh.draw();

	// --- BUDDHA ---
    // Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_buddha.transform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_buddha.transform);

	// Draw mesh
	m_buddha.mesh.draw();

	// --- LUCY ---
    // Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_lucy.transform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_lucy.transform);

	// Draw mesh
	m_lucy.mesh.draw();

#pragma endregion

#pragma region Soulspear
	//m_normalMapShader.bind();

	//// Bind the transform
	//pvm = a_projectionMatrix * a_viewMatrix * m_spear.transform;
	//m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	//m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
	//m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
	//m_normalMapShader.bindUniform("LightColor", m_light.color);
	//m_normalMapShader.bindUniform("LightDirection", m_light.direction);
	//m_normalMapShader.bindUniform("ModelMatrix", m_spear.transform);

	//// Draw the mesh
	//m_spear.mesh.draw();
#pragma endregion

#pragma region Ship
	m_normalMapShader.bind();

	// Bind the transform
	pvm = a_projectionMatrix * a_viewMatrix * m_ship.transform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
	m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
	m_normalMapShader.bindUniform("LightColor", m_light.color);
	m_normalMapShader.bindUniform("LightDirection", m_light.direction);
	m_normalMapShader.bindUniform("ModelMatrix", m_ship.transform);

	// Draw the mesh
	m_ship.mesh.draw();
#pragma endregion

}

void GraphicsProjectApp::UpdateObjectTransforms()
{
	// --- BUNNY ---
	m_bunny.transform = glm::mat4(1.0f);

	m_bunny.transform = glm::translate(m_bunny.transform, m_bunny.position);
	m_bunny.transform = glm::scale(m_bunny.transform, m_bunny.scale);
	m_bunny.transform = glm::rotate(m_bunny.transform, glm::radians(m_bunny.rotation.x), glm::vec3(1, 0, 0));
	m_bunny.transform = glm::rotate(m_bunny.transform, glm::radians(m_bunny.rotation.y), glm::vec3(0, 1, 0));
	m_bunny.transform = glm::rotate(m_bunny.transform, glm::radians(m_bunny.rotation.z), glm::vec3(0, 0, 1));

	// --- DRAGON ---
	m_dragon.transform = glm::mat4(1.0f);

	m_dragon.transform = glm::translate(m_dragon.transform, m_dragon.position);
	m_dragon.transform = glm::scale(m_dragon.transform, m_dragon.scale);
	m_dragon.transform = glm::rotate(m_dragon.transform, glm::radians(m_dragon.rotation.x), glm::vec3(1, 0, 0));
	m_dragon.transform = glm::rotate(m_dragon.transform, glm::radians(m_dragon.rotation.y), glm::vec3(0, 1, 0));
	m_dragon.transform = glm::rotate(m_dragon.transform, glm::radians(m_dragon.rotation.z), glm::vec3(0, 0, 1));


	// --- BUDDHA ---
	m_buddha.transform = glm::mat4(1.0f);

	m_buddha.transform = glm::translate(m_buddha.transform, m_buddha.position);
	m_buddha.transform = glm::scale(m_buddha.transform, m_bunny.scale);
	m_buddha.transform = glm::rotate(m_buddha.transform, glm::radians(m_buddha.rotation.x), glm::vec3(1, 0, 0));
	m_buddha.transform = glm::rotate(m_buddha.transform, glm::radians(m_buddha.rotation.y), glm::vec3(0, 1, 0));
	m_buddha.transform = glm::rotate(m_buddha.transform, glm::radians(m_buddha.rotation.z), glm::vec3(0, 0, 1));


	// --- LUCY ---
	m_lucy.transform = glm::mat4(1.0f);

	m_lucy.transform = glm::translate(m_lucy.transform, m_lucy.position);
	m_lucy.transform = glm::scale(m_lucy.transform, m_lucy.scale);
	m_lucy.transform = glm::rotate(m_lucy.transform, glm::radians(m_lucy.rotation.x), glm::vec3(1, 0, 0));
	m_lucy.transform = glm::rotate(m_lucy.transform, glm::radians(m_lucy.rotation.y), glm::vec3(0, 1, 0));
	m_lucy.transform = glm::rotate(m_lucy.transform, glm::radians(m_lucy.rotation.z), glm::vec3(0, 0, 1));


	// --- SOUL SPEAR ---
	m_spear.transform = glm::mat4(1.0f);

	m_spear.transform = glm::translate(m_spear.transform, m_spear.position);
	m_spear.transform = glm::scale(m_spear.transform, m_spear.scale);
	m_spear.transform = glm::rotate(m_spear.transform, glm::radians(m_spear.rotation.x), glm::vec3(1, 0, 0));
	m_spear.transform = glm::rotate(m_spear.transform, glm::radians(m_spear.rotation.y), glm::vec3(0, 1, 0));
	m_spear.transform = glm::rotate(m_spear.transform, glm::radians(m_spear.rotation.z), glm::vec3(0, 0, 1));

	// Ship
	m_ship.transform = glm::mat4(1.0f);

	m_ship.transform = glm::translate(m_ship.transform, m_ship.position);
	m_ship.transform = glm::scale(m_ship.transform, m_ship.scale);
	m_ship.transform = glm::rotate(m_ship.transform, glm::radians(m_ship.rotation.x), glm::vec3(1, 0, 0));
	m_ship.transform = glm::rotate(m_ship.transform, glm::radians(m_ship.rotation.y), glm::vec3(0, 1, 0));
	m_ship.transform = glm::rotate(m_ship.transform, glm::radians(m_ship.rotation.z), glm::vec3(0, 0, 1));

}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::ColorEdit3("Sunlight Color", &m_light.color[0]);
	ImGui::End();

	ImGui::Begin("Objects");

	if (ImGui::CollapsingHeader(m_bunny.name.c_str())) {
		ImGui::DragFloat3("Position", &m_bunny.position[0], 0.1f);
		ImGui::DragFloat3("Rotation", &m_bunny.rotation[0], 0.1f);
		ImGui::DragFloat3("Scale", &m_bunny.scale[0], 0.1f);
	}

	if (ImGui::CollapsingHeader(m_dragon.name.c_str())) {
		ImGui::DragFloat3("Position", &m_dragon.position[0], 0.1f);
		ImGui::DragFloat3("Rotation", &m_dragon.rotation[0], 0.1f);
		ImGui::DragFloat3("Scale", &m_dragon.scale[0], 0.1f);
	}

	if (ImGui::CollapsingHeader(m_buddha.name.c_str())) {
		ImGui::DragFloat3("Position", &m_buddha.position[0], 0.1f);
		ImGui::DragFloat3("Rotation", &m_buddha.rotation[0], 0.1f);
		ImGui::DragFloat3("Scale", &m_buddha.scale[0], 0.1f);
	}

	if (ImGui::CollapsingHeader(m_lucy.name.c_str())) {
		ImGui::DragFloat3("Position", &m_lucy.position[0], 0.1f);
		ImGui::DragFloat3("Rotation", &m_lucy.rotation[0], 0.1f);
		ImGui::DragFloat3("Scale", &m_lucy.scale[0], 0.1f);
	}

	if (ImGui::CollapsingHeader(m_spear.name.c_str())) {
		ImGui::DragFloat3("Position", &m_spear.position[0], 0.1f);
		ImGui::DragFloat3("Rotation", &m_spear.rotation[0], 0.1f);
		ImGui::DragFloat3("Scale", &m_spear.scale[0], 0.1f);
	}

	UpdateObjectTransforms();

	ImGui::End();
}
