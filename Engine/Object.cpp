#include "Transform.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Component.h"
//#include "PhysicsWorld.h"
#include "World.h"
#include "RigidBodyRegistry.h"
#include "Object.h"

Object::Object(std::unique_ptr<Transform> transform, std::unique_ptr<MeshRenderer> meshRenderer, Object* parent)
	: mTransform{ std::move(transform) }, mMeshRenderer{ std::move(meshRenderer) }, mParentObject{ parent }
{
	mTransform.get()->setParentObject(this);
	mMeshRenderer.get()->setParent(this);
	mTransform.get()->setScale(mMeshRenderer.get()->getMesh()->scalingFactor);
}

void Object::addComponent(std::unique_ptr<Component> component)
{
	//check if component is null
	if (!component)
		throw std::runtime_error("Component is null");
	//check if component already exists
	for (auto& existingComponent : m_components)
	{
		if (existingComponent.get() == component.get())
			throw std::runtime_error("Component already exists");
	}

	m_components.push_back(std::move(component));
	m_components.back().get()->setParentObject(this);

	//if currently added component is RigidBody, finalize it
	if (auto* casted = dynamic_cast<RigidBody*>(m_components.back().get()))
	{
		if (!getComponent<Collider>())
			throw std::runtime_error("Can't initialize RigidBody without collider");
		casted->finalizeRigidBody();
		RigidBodyRegistry::queueRigidBody(casted, 1);
	}
}

