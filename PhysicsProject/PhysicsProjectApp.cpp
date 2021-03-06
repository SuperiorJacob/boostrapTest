#include "PhysicsProjectApp.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include "Pachinko.h"
#include "Highscores.h"
#include <Gizmos.h>
#include <string>

PhysicsProjectApp::PhysicsProjectApp()
{

}

PhysicsProjectApp::~PhysicsProjectApp()
{
}

bool PhysicsProjectApp::startup() {

	// HS
	m_highscores = new Highscores();
	m_highscores->SetGameName("JacobsGame");
	m_highscores->Load();
	//

	// Increases 2D line coun to maximise the number of objects we can draw.
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	//m_font = new aie::Font("./font/consolas.ttf", 32);
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();

	m_physicsScene->SetGravity(glm::vec2(0, -30));

	// Lower the valu, the more accurate the simulation will be;
	// but it will increase the processing time required.
	// If it is too high it causes the sim to stutter and reduce stability.
	m_physicsScene->SetTimeStep(0.01f);

	/*DrawRect();*/
	/*SphereAndPlane();*/
	//SpringTest(10);
	/*TriggerTest();*/

	std::vector<PhysicsObject*> pachinko_map {  
	};

	bool spacing = false;

	Sphere* balls[9][8];

	for (int x = -4; x < 5; x++)
	{

		for (int y = 0; y < 8; y++)
		{
			spacing = (y % 2 == 0);
			int mix = rand() % 5;

			// Default

			Sphere* ball = new Sphere(glm::vec2((spacing ? 3 : -3) + (-x * 10), 40 - y * 10), glm::vec2(0), (mix == 1) ? 100.0f : 3.0f, 1.3f, (mix == 1) ? glm::vec4(1, 1, 0.5, 1) : glm::vec4(0.5, 0.5, 0.5, 1));
			ball->SetElasticity((mix == 1) ? 0.1f : 1.f);
			ball->SetTag("Static");

			pachinko_map.push_back(ball);

			// Spring

			/*Sphere* ball = new Sphere(glm::vec2((spacing ? 2.5 : -2.5) + (-x * 10), 40 - y * 10), glm::vec2(0), 1.f, 1.f, (mix == 1) ? glm::vec4(1, 1, 0.5, 1) : glm::vec4(1, 1, 1, 1));
			ball->SetElasticity((mix == 1) ? 0.1f : 1.f);
			ball->SetTag("Static");

			if (y == 0)
			{
				ball->SetKinematic(true);
			} len was here

			m_physicsScene->AddActor(ball);

			int xPos = x + 4;

			balls[xPos][y] = ball;

			if (((xPos - 1) >= 0) && ((y) >= 0) && balls[xPos - 1][y])
			{
				m_physicsScene->AddActor(new Spring(ball, balls[xPos - 1][y], 10, 500));
			}

			if (((xPos) >= 0) && ((y - 1) >= 0) && balls[xPos][y - 1])
			{
				m_physicsScene->AddActor(new Spring(ball, balls[xPos][y - 1], 10, 500));
			}

			if (((xPos - 1) >= 0) && ((y - 1) >= 0) && balls[xPos - 1][y - 1])
			{
				m_physicsScene->AddActor(new Spring(ball, balls[xPos - 1][y - 1], 10, 500));
			}*/
		}

	}

	for (int x = -4; x < 5; x++)
	{
		int score = (5 - std::abs(x)) * 10;

		Box* pointBox = new Box(glm::vec2(-x * 10, -50), glm::vec2(0, 0), 0.f, 1.0f, 4.5f, 5.f, glm::vec4(1.2f - std::abs(x)/4.f, 0, 0, 1));
		pointBox->SetKinematic(true);
		pointBox->SetTrigger(true);

		pointBox->SetTag(std::to_string(score));

		pointBox->triggerEnter = [=](PhysicsObject* other) { 
			if (other->GetTag() != "Static")
			{
				m_pachinko->AddScore(score);
				m_highscores->AddHighScore(m_pachinko->GetScore());

				Rigidbody* box = dynamic_cast<Rigidbody*>(other);

				if (box != nullptr)
					box->SetPosition(glm::vec2(0, -100));
			}
		};

		pachinko_map.push_back(pointBox);
	}

	Box* boxe = new Box(glm::vec2(60, -40), glm::vec2(0, 0), -12.f, 100.f, 15.5f, 3.f, glm::vec4(1, 1, 0, 1));
	boxe->SetKinematic(true);

	pachinko_map.push_back(boxe);

	Box* boxe1 = new Box(glm::vec2(-60, -40), glm::vec2(0, 0), 12.f, 100.f, 15.5f, 3.f, glm::vec4(1, 1, 0, 1));
	boxe1->SetKinematic(true);

	pachinko_map.push_back(boxe1);

	//Box* boxe2 = new Box(glm::vec2(0, 0), glm::vec2(0, 0), 0.785398f, 30.f, 5.f, 5.f, glm::vec4(1, 1, 1, 1));
	//boxe2->SetKinematic(true);

	//pachinko_map.push_back(boxe2);

	//Box* boxe2 = new Box(glm::vec2(0, 30), glm::vec2(0, 0), 12.f, 0.1f, 10.f, 0.1f, glm::vec4(1, 0.5f, 0, 1));
	//boxe2->SetKinematic(true);

	//pachinko_map.push_back(boxe2);

	//m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -55));

	m_pachinko = new Pachinko(m_physicsScene, pachinko_map);
	m_pachinko->Start(0.0f);

	return true;
}

void PhysicsProjectApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsProjectApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	if (input->isMouseButtonDown(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
		aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(0.7f));
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (m_pachinko != nullptr && m_pachinko->Started())
	{
		m_pachinko->Update(deltaTime);
	}
}

void PhysicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// If X-axis = -100 to 100, Y-axis = -56.25 to 56.25
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));
	
	// draw your stuff here!
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);

	// Pachinko
	char score[64];
	sprintf_s(score, 64, "SCORE: %i", m_pachinko->GetScore());
	m_2dRenderer->drawText(m_font, score, 0, 720 - 64);

	char balls[64];
	sprintf_s(balls, 64, "BALLS LEFT: %i", m_pachinko->GetBallsLeft());
	m_2dRenderer->drawText(m_font, balls, 0, 720 - 96);

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	m_highscores->Draw(m_font, m_2dRenderer, 0, 720 - 160);

	// done drawing sprites
	m_2dRenderer->end();
}

glm::vec2 PhysicsProjectApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	// We will move the center of the screen to (0, 0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// Scale this according to the extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}

void PhysicsProjectApp::DrawRect()
{
	m_physicsScene->AddActor(new Sphere(glm::vec2(20, 10), glm::vec2(-10, -17), 1, 3, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Plane(glm::vec2(-0.65, 0.75), -25));

	Box* box1 = new Box(glm::vec2(-20, 0), glm::vec2(16, -4), 1, 4, 8, 4, glm::vec4(1, 0, 0, 1));
	box1->SetRotation(0.5f);

	Box* box2 = new Box(glm::vec2(10, 0), glm::vec2(-4, 0), 1, 4, 8, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);

	box1->ApplyForce(glm::vec2(30, 0), glm::vec2(0));
	box2->ApplyForce(glm::vec2(-15, 0), glm::vec2(0));

	Sphere* ball = new Sphere(glm::vec2(5, 10), glm::vec2(0), 1, 3, glm::vec4(0, 0, 1, 1));
	ball->SetRotation(0.5);
	m_physicsScene->AddActor(ball);
	ball->SetKinematic(true);
}

void PhysicsProjectApp::SphereAndPlane()
{
	Sphere* ball1;
	ball1 = new Sphere(glm::vec2(-40, 40), glm::vec2(0), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball1);
	ball1->ApplyForce(glm::vec2(50, 30), glm::vec2(0));

	Sphere* ball2;
	ball2 = new Sphere(glm::vec2(40, 40), glm::vec2(0), 3.0f, 1, glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(ball2);
	ball2->ApplyForce(glm::vec2(-50, 30), glm::vec2(0));

	Plane* plane = new Plane();
	m_physicsScene->AddActor(plane);
}

void PhysicsProjectApp::SpringTest(int a_amount)
{
	Sphere* prev = nullptr;
	for (int i = 0; i < a_amount; i++)
	{
		// This will need to spawn the sphere at the bottom of the previous one,
		// to make a pendulum that is effected by gravity
		Sphere* sphere = new Sphere(glm::vec2(i * 3, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(0, 0, 1, 1));

		if (i == 0)
		{
			sphere->SetKinematic(true);
		}

		m_physicsScene->AddActor(sphere);

		if (prev)
		{
			m_physicsScene->AddActor(new Spring(sphere, prev, 10, 500));
		}

		prev = sphere;
	}

	Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 0.3f, 20, 8, 2);
	box->SetKinematic(true);
	m_physicsScene->AddActor(box);
}

void PhysicsProjectApp::TriggerTest()
{
	Sphere* ball1 = new Sphere(glm::vec2(-20, 0), glm::vec2(10), 4, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(10, -20), glm::vec2(-10), 4, 4, glm::vec4(0, 0.5, 0.5, 1));

	ball2->SetKinematic(true);
	ball2->SetTrigger(false);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));
	m_physicsScene->AddActor(new Box(glm::vec2(20, 10), glm::vec2(8, 0), 0.5, 4, 8, 4));
	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(-8, 0), 0.5, 4, 8, 4));

	ball2->triggerEnter = [=](PhysicsObject* other) { std::cout << "Entered: " << other << std::endl; };
	ball2->triggerExit = [=](PhysicsObject* other) { std::cout << "Exited: " << other << std::endl; };
}
