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

	m_camera.Update(deltaTime);

	IMGUI_Logic();

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

	m_scene->Draw();

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
	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_bunnyShader.link())
	{
		printf("Bunny Shader had an error: %s\n", m_bunnyShader.getLastError());
		return false;
	}

	if (m_bunny.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Failed!\n");
		return false;
	}

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
		printf("Buddha Shader had an error: %s\n", m_lucyShader.getLastError());
		return false;
	}

	if (m_lucy.load("./stanford/lucy.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
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

	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));

	// Yes horrific
	Instance* inst = new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), &m_spear, &m_normalMapShader);
	inst->m_name = "Spear";
	m_scene->AddInstance(inst);

	//Instance* inst1 = new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), &m_bunny, &m_bunnyShader);
	//inst1->m_name = "Bunny";
	//m_scene->AddInstance(inst1);

	//Instance* inst2 = new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), &m_dragon, &m_dragonShader);
	//inst2->m_name = "Dragon";
	//m_scene->AddInstance(inst2);

	//Instance* inst3 = new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), &m_lucy, &m_lucyShader);
	//inst3->m_name = "Lucy";
	//m_scene->AddInstance(inst3);

	Instance* inst4 = new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), &m_barrel, &m_normalMapShader);
	inst4->m_name = "Barrel";
	m_scene->AddInstance(inst4);
	//

	// Add a red light on the left side
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));

	// Add a green light on the right side
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));

	return true;
}

//void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
//{
//	// PVM = Projection View Matrix
//	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);
//
//#pragma region Quad
//	// Bind the shader
//	m_textureShader.bind();
//
//	// Bind the transform of the mesh
//	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform;
//	m_textureShader.bindUniform("ProjectionViewModel", pvm);
//
//	// Bind the texture to a location of your choice (0)
//	m_textureShader.bindUniform("diffuseTexture", 0);
//
//	// Bind the texture to the specificed location
//	m_gridTexture.bind(0);
//
//	// Draw the quad...
//	m_quadMesh.Draw();
//
//#pragma endregion
//
//#pragma region FlatBunny
//	//m_bunnyShader.bind();
//	//pvm = a_projectionMatrix * a_viewMatrix * m_bunny.transform;
//	//m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
//	//m_bunnyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));
//
//	// Draw bunny mesh
//	// m_bunny.mesh.draw();
//
//#pragma endregion
//
//#pragma region Dragon
//	//m_dragonShader.bind();
//	//pvm = a_projectionMatrix * a_viewMatrix * m_dragon.transform;
//	//m_dragonShader.bindUniform("ProjectionViewModel", pvm);
//	//m_dragonShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));
//
//	// Draw dragon mesh
//	// m_dragon.mesh.draw();
//
//#pragma endregion
//
//#pragma region Buddha
//	//m_buddhaShader.bind();
//	//pvm = a_projectionMatrix * a_viewMatrix * m_buddha.transform;
//	//m_buddhaShader.bindUniform("ProjectionViewModel", pvm);
//	//m_buddhaShader.bindUniform("MeshFlatColor", glm::vec4(1, 0, 1, 1));
//
//	// Draw buddha mesh
//	// m_buddha.mesh.draw();
//
//#pragma endregion
//
//#pragma region Lucy
//	//m_lucyShader.bind();
//	//pvm = a_projectionMatrix * a_viewMatrix * m_lucy.transform;
//	//m_lucyShader.bindUniform("ProjectionViewModel", pvm);
//	//m_lucyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 1, 1));
//
//	// Draw lucy mesh
//	// m_lucy.mesh.draw();
//
//#pragma endregion
//
//#pragma region Phong
//	//// Bind the shader
//	//m_phongShader.bind();
//
//	//// Bind the camera position
//	//m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));
//
//	//// Bind the light
//	//m_phongShader.bindUniform("AmbientColor", m_ambientLight);
//	//m_phongShader.bindUniform("LightColor", m_light.color);
//	//m_phongShader.bindUniform("LightDirection", m_light.direction);
//
//	//// --- BUNNY ---
//	//// Bind the PVM
//	//pvm = a_projectionMatrix * a_viewMatrix * m_bunny.transform;
//	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	//// Bind the lighting transforms
//	//m_phongShader.bindUniform("ModelMatrix", m_bunny.transform);
//
//	//// Draw mesh
//	//m_bunny.mesh.draw();
//
//	//// --- DRAGON ---
//	//// Bind the PVM
//	//pvm = a_projectionMatrix * a_viewMatrix * m_dragon.transform;
//	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	//// Bind the lighting transforms
//	//m_phongShader.bindUniform("ModelMatrix", m_dragon.transform);
//
//	//// Draw mesh
//	//m_dragon.mesh.draw();
//
//	//// --- BUDDHA ---
// //   // Bind the PVM
//	//pvm = a_projectionMatrix * a_viewMatrix * m_buddha.transform;
//	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	//// Bind the lighting transforms
//	//m_phongShader.bindUniform("ModelMatrix", m_buddha.transform);
//
//	//// Draw mesh
//	//m_buddha.mesh.draw();
//
//	//// --- LUCY ---
// //   // Bind the PVM
//	//pvm = a_projectionMatrix * a_viewMatrix * m_lucy.transform;
//	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	//// Bind the lighting transforms
//	//m_phongShader.bindUniform("ModelMatrix", m_lucy.transform);
//
//	//// Draw mesh
//	//m_lucy.mesh.draw();
//
//#pragma endregion
//
//#pragma region Soulspear
//	m_normalMapShader.bind();
//
//	// Bind the transform
//	pvm = a_projectionMatrix * a_viewMatrix * m_spear.transform;
//	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
//	m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
//	m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
//	m_normalMapShader.bindUniform("LightColor", m_light.color);
//	m_normalMapShader.bindUniform("LightDirection", m_light.direction);
//	m_normalMapShader.bindUniform("ModelMatrix", m_spear.transform);
//
//	// Draw the mesh
//	m_spear.mesh.draw();
//#pragma endregion
//
//#pragma region Knife
//	//m_normalMapShader.bind();
//
//	//// Bind the transform
//	//pvm = a_projectionMatrix * a_viewMatrix * m_barrel.transform;
//	//m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
//	//m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
//	//m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
//	//m_normalMapShader.bindUniform("LightColor", m_light.color);
//	//m_normalMapShader.bindUniform("LightDirection", m_light.direction);
//	//m_normalMapShader.bindUniform("ModelMatrix", m_barrel.transform);
//
//	//// Draw the mesh
//	//m_barrel.mesh.draw();
//#pragma endregion
//
//#pragma region LoadAll
//	//for (auto& obj : m_objectList) 
//	//{
//	//	m_normalMapShader.bind();
//
//	//	pvm = a_projectionMatrix * a_viewMatrix * obj->transform;
//	//	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
//	//	m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
//	//	m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
//	//	m_normalMapShader.bindUniform("LightColor", m_light.color);
//	//	m_normalMapShader.bindUniform("LightDirection", m_light.direction);
//	//	m_normalMapShader.bindUniform("ModelMatrix", obj->transform);
//
//	//	obj->mesh.draw();
//	//}
//
//#pragma endregion
//
//}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::ColorEdit3("Sunlight Color", &m_scene->GetLight().m_color[0]);
	ImGui::End();

	m_scene->IMGUI_Logic();
}
