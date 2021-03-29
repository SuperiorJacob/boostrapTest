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

	int c = 0;

	if (ImGui::CollapsingHeader("Game Objects"))
	{
		ImGui::Indent();
		for (auto i = m_instances.begin(); i != m_instances.end(); i++)
		{
			c++;
			Instance* instance = *i;
			instance->Draw(this);

			ImGui::PushID(c);
			if (ImGui::CollapsingHeader(instance->m_actualName))
			{
				glm::quat rotation;
				glm::vec3 position, skew, scale;
				glm::vec4 perspective;
				glm::decompose(instance->GetTransform(), scale, rotation, position, skew, perspective);

				ImGui::InputText("Name", instance->m_name, 32);
				if (ImGui::Button("Update Name", { 150,20 }))
				{
					strcpy(instance->m_actualName, instance->m_name);
				}

				ImGui::DragFloat3("Position", &position[0], 0.1f);
				ImGui::DragFloat3("Rotation", &instance->m_rotation[0], 0.1f);
				ImGui::DragFloat3("Scale", &scale[0], 0.1f, 0.01f);

				instance->SetTransform(instance->MakeTransform(position, instance->m_rotation, scale));
			}
			ImGui::PopID();
		}

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Lights"))
	{
		ImGui::Indent();
		for (int i = 0; i < m_pointLights.size(); i++)
		{
			c++;
			Light light = m_pointLights[i];

			ImGui::PushID(c);
			if (ImGui::CollapsingHeader(light.m_actualName))
			{
				ImGui::InputText("Name", light.m_name, 32);
				if (ImGui::Button("Update Name", { 150,20 }))
				{
					strcpy(light.m_actualName, light.m_name);
				}

				ImGui::DragFloat3("Position", &light.m_direction[0], 0.1f);
				ImGui::DragFloat3("Colour", &light.m_untouchedColor[0], 0.1f, 0, 1);
				ImGui::DragFloat("Scale", &light.m_intensity, 0.1f, 0);

				light.m_color = light.m_untouchedColor * light.m_intensity;
			}

			m_pointLights[i] = light;

			ImGui::PopID();
		}
		ImGui::Unindent();
	}
	ImGui::End();
}
