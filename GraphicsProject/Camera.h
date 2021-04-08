/*

* File:			Camera.h
* Author:		Jacob Cooper (s200503@students.aie.edu.au)

* Edit Dates:
*	First:		12/03/2021
*	Last:		8/04/2021

* Summary:
*	Used to simulate and control a camera of any form.

*/

#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 a_position, float a_xRot, float a_yRot, bool a_isFlybyCam);
	~Camera() {};

	void Update(float a_deltaTime);
	
	glm::vec3 GetPosition() { return m_position; }
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float a_width, float a_height);

private:
	float m_theta;  // In degrees
	float m_phi;    // In degrees
	glm::vec3 m_position;

	bool m_isFlybyCam;

	float m_lastMouseX;
	float m_lastMouseY;
};
