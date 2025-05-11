#pragma once

#include "Collider.h"

class PlaneCollider : public Collider
{
private:
	btVector3 m_normal;
	float m_distance;

public:
	PlaneCollider(/*const glm::vec3& center, Material& material,*/
		const btVector3 normal, float distance)
		: /*Collider(center, material),*/ m_normal{ normal }, m_distance{ distance }
	{
		assignColliderShape(std::make_unique<btStaticPlaneShape>(normal, distance));
	}
};