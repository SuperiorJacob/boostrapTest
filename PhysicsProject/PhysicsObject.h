#pragma once
#include <glm/glm.hpp>
#include <string>

// This is a pure abstract class.
// It will be used as a base for the derived classes that will use FixedUpdate

enum ShapeType
{
	JOINT = -1,
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
public:
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep) = 0;
	virtual void MakeGizmo() = 0;
	virtual void Debug() = 0;
	virtual void ResetPosition() {};

	virtual bool IsInside(glm::vec2 a_point) = 0;

	ShapeType GetShapeID() { return m_shapeID; }

	bool IsKinematic() { return m_isKinematic; }
	bool SetKinematic(bool a_state) { return m_isKinematic = a_state; }

	float GetElasticity() { return m_elasticity; }

	// Keep to 1 for realism.
	void SetElasticity(float a_new) { m_elasticity = a_new; }
	
	std::string GetTag() { return m_tag; }

	void SetTag(std::string a_tag) { m_tag = a_tag; }
protected:
	ShapeType m_shapeID;

	bool m_isKinematic;
	float m_elasticity;

	std::string m_tag = "";

	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
};

