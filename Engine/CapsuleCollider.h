#pragma once

#include "Collider.h"

class CapsuleCollider : public Collider
{
private:
	float m_radius;
	float m_height;
public:
	CapsuleCollider(float radius, float height) :
		m_radius{ radius }, m_height{ height }
	{
		assignColliderShape(std::make_unique<btCapsuleShape>(radius, height));
	};
};