#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
private:
	float m_size;
public:
	BoxCollider(/*const glm::vec3& center, Material& material, */float size)
		: /*Collider(center, material),*/ m_size{ size }
	{
		assignColliderShape(std::make_unique<btBoxShape>(btVector3(size, size, size)));
	}

	BoxCollider(float dimX, float dimY, float dimZ)
	{
		assignColliderShape(std::make_unique<btBoxShape>(btVector3(dimX, dimY, dimZ)));
	}

	void setSize(float size) { m_size = size; }
	float getSize() const { return m_size; }
};
