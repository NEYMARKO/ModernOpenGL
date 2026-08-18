#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include "../../Material.h"
#include "../Component.h";

class Collider : public Component
{
private:
	glm::vec3 m_center;
	//material will be shared among all colliders - default material should be made
	/*Material m_material;*/
	std::unique_ptr<btCollisionShape> m_collisionShape;
public:
	Collider() = default;
	//Collider(const glm::vec3& center/*, const Material& material*/)
	//	: m_center{ center }/*, m_material{ material }*/
	//{
	//}
	virtual void* getBase(std::type_index t) override
	{
		if (t == typeid(Collider)) return this;
		return nullptr;
	}
	void assignColliderShape(std::unique_ptr<btCollisionShape> colliderShape)
	{
		m_collisionShape = std::move(colliderShape);
	}
	virtual void alignBoundsToObject() {};
	btCollisionShape* getCollisionShape() { return m_collisionShape.get(); }
};