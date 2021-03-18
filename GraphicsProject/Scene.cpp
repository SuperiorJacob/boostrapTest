#include "Scene.h"
#include "Instance.h"
#include <imgui.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL 1

#include <glm/gtx/matrix_decompose.hpp>

Scene::Scene(Camera* a_camera, glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight)
	: m_camera(a_camera), m_windowSize(a_windowSize), m_light(a_light), m_ambientLight(a_ambientLight)
{
}

Scene::~Scene()
{
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
		delete (*i);
}

void Scene::AddInstance(Instance* a_instance)
{
	m_instances.push_back(a_instance);

}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].m_direction;
		m_pointLightColors[i] = m_pointLights[i].m_color;
	}

	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		Instance* instance = *i;
		instance->Draw(this);
	}
}

void Scene::IMGUI_Logic()
{
	ImGui::Begin("Objects");

	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		Instance* instance = *i;
		instance->Draw(this);

		if (ImGui::CollapsingHeader(instance->m_name))
		{
			glm::quat rotation;
			glm::vec3 position, skew, scale;
			glm::vec4 perspective;
			glm::decompose(instance->GetTransform(), scale, rotation, position, skew, perspective);

			//ImGui::InputText("Name", instance->m_name, std::strlen(instance->m_name));

			ImGui::DragFloat3("Position", &position[0], 0.1f);
			ImGui::DragFloat3("Rotation", &rotation[0], 0.1f);
			ImGui::DragFloat3("Scale", &scale[0], 0.1f);

			instance->SetTransform(instance->MakeTransform(position, rotation * glm::vec3(1), scale));
		}
	}

	ImGui::End();
}
