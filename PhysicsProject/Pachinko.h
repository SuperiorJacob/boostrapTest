#pragma once
#include "PhysicsObject.h"
#include "PhysicsScene.h"
#include "Box.h"
#include <vector>

class Pachinko
{
public:
	Pachinko(PhysicsScene* a_scene, std::vector<PhysicsObject*> a_map);
	~Pachinko();

	void Start(float a_timer);
	void Update(float deltaTime);
	void End();

	bool Started() { return m_started; }

	void SetScore(int a_score) { m_score = a_score; }
	void AddScore(int a_score) { m_score += a_score; }
	int GetScore() { return m_score; }

	void SetMap(std::vector<PhysicsObject*> a_map) { m_map = a_map; }
	std::vector<PhysicsObject*> GetMap() { return m_map; }

	int GetBallsLeft() { return m_ballsLeft; }
	void SetBallsLeft(int a_ballsLeft) { m_ballsLeft = a_ballsLeft; }

protected:
	int m_score = 0;

	float m_timer = 0.0f;
	float m_nextBallSpawn = 0.f;

	float m_dropperSpeed = 35.f;
	bool left = false;

	bool m_started = false;

	Box* m_ballDropper;

	int m_ballsLeft = 100;

	std::vector<PhysicsObject*> m_balls;
	std::vector<PhysicsObject*> m_map;

	PhysicsScene* m_physicsScene = nullptr;
};

