#include "Transform.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Component.h"
//#include "PhysicsWorld.h"
#include "World.h"
#include "RigidBodyRegistry.h"
#include "Object.h"


Object::Object(Transform&& transform, MeshRenderer&& meshRenderer, 
	const std::string& name, Object* parent)
	: m_transform{ std::move(transform) }, m_meshRenderer{ std::move(meshRenderer) }, 
	m_parentObject{ parent }, m_name { name }
{
	m_transform.setParentObject(this);
	m_meshRenderer.setParentObject(this);
	m_transform.setScale(m_meshRenderer.getMesh()->scalingFactor);
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

