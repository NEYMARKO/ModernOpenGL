#pragma once

#include "./Components/Collider.h"
#include "Object.h"
#include "./Components/Transform.h"
#include "./Components/MeshRenderer.h"
#include "Mesh.h"
class SphereCollider : public Collider
{
private:
	float m_radius;
public:
	SphereCollider(/*const glm::vec3& center, Material& material, */float radius)
		: /*Collider(center, material),*/ m_radius{ radius }
	{
		assignColliderShape(std::make_unique<btSphereShape>(radius));
	}
	virtual void* getBase(std::type_index t) override
	{
		if (t == typeid(SphereCollider)) return this;
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
		//btVector3 dimensions = t->getScale() * 1 / 
		float comparison1 = std::max(dimensions.x, dimensions.y);
		float r = std::max(comparison1, dimensions.z);
		assignColliderShape(std::make_unique<btSphereShape>(r));
	}
	void setRadius(float radius) { m_radius = radius; }
	float getRadius() const { return m_radius; }
};