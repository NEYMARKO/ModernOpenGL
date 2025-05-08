#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
private:
	float m_radius;
public:
	SphereCollider(const glm::vec3& center, Material& material, float radius)
		: Collider(center, material), m_radius{ radius }
	{
		assignColliderShape(std::make_unique<btSphereShape>(radius));
	}

	void setRadius(float radius) { m_radius = radius; }
	float getRadius() const { return m_radius; }
};