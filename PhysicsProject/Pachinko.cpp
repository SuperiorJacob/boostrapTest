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

	m_ballsLeft = 100;

	m_started = true;
}

glm::vec4 typeCols[4] = {
	{0.5, 0.5, 0.5, 1},
	{1, 1, 0.5, 1},
	{0, 0.5, 0.5, 1},
	{1, 1, 1, 1},
};

void Pachinko::Update(float deltaTime)
{
	m_timer -= 1 * deltaTime;
	glm::vec2 ballPos = m_ballDropper->GetPosition();

	m_ballDropper->SetPosition(glm::vec2(ballPos.x + (left ? -m_dropperSpeed : m_dropperSpeed) * deltaTime, ballPos.y));

	if (ballPos.x >= 90) left = true;
	else if (ballPos.x <= -90) left = false;

	aie::Input* input = aie::Input::getInstance();

	if (m_nextBallSpawn >= m_timer && input->isKeyDown(aie::EInputCodes::INPUT_KEY_SPACE) && m_ballsLeft > 0)
	{
		m_nextBallSpawn = m_timer - 5.f * deltaTime;
		m_ballsLeft -= 1;

		Sphere* sphere = new Sphere(m_ballDropper->GetPosition() + glm::vec2(0, -2), glm::vec2(0, 0), 10.0f, 1, glm::vec4(0, 1, 0, 1));
		sphere->SetElasticity(1.f);
		sphere->m_collisionCallback = [=](PhysicsObject* other) {
			if (other->GetTag() == "Static" && sphere->GetTag() != "Dead" && sphere->m_lastHit != other)
			{
				Sphere* box = dynamic_cast<Sphere*>(other);

				if (box != nullptr)
				{
					int mix = rand() % 4;
					glm::vec4 col = typeCols[0];
					float mass = 1.f;
					float radius = 1.3f;
					box->m_collisionCallback = {};

					switch (mix)
					{
					case 1:
						col = typeCols[0];
						mass = 3.0f;

						break;
					case 2:
						col = typeCols[1];
						mass = 100.0f;

						break;
					case 3:
						col = typeCols[2];
						mass = 0.0f;
						radius = 2.0f;

						break;
					case 4:
						col = typeCols[3];

						/*other->SetTag("Web");

						box->m_collisionCallback = [=](PhysicsObject* other2)
						{
							if (other->GetTag() == "Web" && other2->GetTag() != "Dead")
							{
								other2->SetTag("Dead");
								m_physicsScene->AddActor(new Spring(box, dynamic_cast<Rigidbody*>(other2), 10.f, 200.f));
							}
						};*/

						break;

					default:
						break;
					}

					box->SetColour(col);

					Rigidbody* rig = dynamic_cast<Rigidbody*>(other);
					rig->SetMass(mass);
				}
			}

		};
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
