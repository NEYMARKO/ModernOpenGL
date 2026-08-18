#pragma once

#include "./Components/Collider.h"

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
	virtual void* getBase(std::type_index t) override
	{
		if (t == typeid(CapsuleCollider)) return this;
		return Collider::getBase(t);
	}
};