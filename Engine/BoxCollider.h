#pragma once

#include "Collider.h"
#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Mesh.h"
class BoxCollider : public Collider
{
private:
	float m_size{ 1.0f };
public:

	BoxCollider() {}
	BoxCollider(/*const glm::vec3& center, Material& material, */float size)
		: /*Collider(center, material),*/ m_size{ size }
	{
		assignColliderShape(std::make_unique<btBoxShape>(btVector3(size, size, size)));
	}

	BoxCollider(float dimX, float dimY, float dimZ)
	{
		assignColliderShape(std::make_unique<btBoxShape>(btVector3(dimX, dimY, dimZ)));
	}
	virtual void* getBase(std::type_index t) override
	{
		if (t == typeid(BoxCollider)) return this;
		return Collider::getBase(t);
	}
	virtual void alignBoundsToObject() override
	{
		Transform* t = getParentObject()->getComponent<Transform>();
		Mesh* m = getParentObject()->getComponent<MeshRenderer>()->getMesh();
		glm::vec3 maximums = m->m_maximums;
		glm::vec3 minimums = m->m_minimums;
		glm::vec3 scale = t->getScale();
		glm::vec3 dimensions = (maximums - minimums) / 2.0f * scale;
		btVector3 btDimensions = t->glmToBulletVec3(dimensions);
		assignColliderShape(std::make_unique<btBoxShape>(
			btDimensions));
	}
	void setSize(float size) { m_size = size; }
	float getSize() const { return m_size; }
};
