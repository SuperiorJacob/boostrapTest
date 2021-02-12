#include "Pachinko.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include <algorithm>

#include "PhysicsScene.h"

Pachinko::Pachinko(PhysicsScene* a_scene, std::vector<PhysicsObject*> a_map)
{
	m_physicsScene = a_scene;
	m_map = a_map;

	for each (auto obj in a_map)
	{
		obj->SetKinematic(true);
		obj->SetElasticity(1.f);
		m_physicsScene->AddActor(obj);
	}
}

Pachinko::~Pachinko()
{
	End();
}

void Pachinko::Start(float a_timer)
{
	m_ballDropper = new Box(glm::vec2(0, 50), glm::vec2(0, 0), 0.f, 1.0f, 5.f, 1.f, glm::vec4(1, 1, 1, 1));
	m_ballDropper->SetKinematic(true);

	m_physicsScene->AddActor(m_ballDropper);

	m_started = true;
}

void Pachinko::Update(float deltaTime)
{
	m_timer -= 1;

	glm::vec2 ballPos = m_ballDropper->GetPosition();

	m_ballDropper->SetPosition(glm::vec2(ballPos.x + (left ? -m_dropperSpeed : m_dropperSpeed) * deltaTime, ballPos.y));

	if (ballPos.x >= 90) left = true;
	else if (ballPos.x <= -90) left = false;

	aie::Input* input = aie::Input::getInstance();

	if (m_nextBallSpawn >= m_timer && input->isKeyDown(aie::EInputCodes::INPUT_KEY_SPACE))
	{
		m_nextBallSpawn = m_timer - 5.f;

		Sphere* sphere = new Sphere(m_ballDropper->GetPosition() + glm::vec2(0, -2), glm::vec2(0, 0), 10.0f, 1, glm::vec4(0, 1, 0, 1));
		sphere->SetElasticity(1.f);
		m_physicsScene->AddActor(sphere);

		m_balls.push_back(sphere);
	}

	for each (auto obj in m_balls)
	{
		Rigidbody* ball = dynamic_cast<Rigidbody*>(obj);

		glm::vec2 pos = ball->GetPosition();

		if (pos.x > 100 || pos.x < -100 || pos.y > 100 || pos.y < -100)
		{
			m_physicsScene->RemoveActor(ball);
		}
	}

	m_balls.erase(std::remove_if(m_balls.begin(), m_balls.end(),
		[](auto obj) {
			Rigidbody* ball = dynamic_cast<Rigidbody*>(obj);

			glm::vec2 pos = ball->GetPosition();

			return (pos.x > 100 || pos.x < -100 || pos.y > 100 || pos.y < -100);
		}), m_balls.end());
}

void Pachinko::End()
{
	m_physicsScene->RemoveActor(m_ballDropper);
	delete m_ballDropper;
}
