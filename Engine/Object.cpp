#include "./Components/Transform.h"
#include "./Components/MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "./Components/RigidBody.h"
#include "./Components/Collider.h"
#include "SphereCollider.h"
#include "./Components/Component.h"
//#include "PhysicsWorld.h"
#include "World.h"
#include "RigidBodyRegistry.h"
#include "Object.h"


Object::Object(Transform&& transform, MeshRenderer&& meshRenderer, 
	const std::string& name, SceneEntity* parent)
	: SceneEntity{ std::move(
		EditorCollider { meshRenderer.getMesh()->m_minimums, meshRenderer.getMesh()->m_maximums }), 
	std::move( transform ), name, parent },
	m_meshRenderer{ std::move(meshRenderer) }
{
	m_transform.setParentObject(this);
	m_transform.setScale(m_meshRenderer.getMesh()->scalingFactor * getComponent<Transform>()->getScale());
	m_meshRenderer.setParentObject(this);
	m_editorCollider.setParent(this);
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
	
	if (auto* casted = dynamic_cast<Collider*>(m_components.back().get()))
	{
		casted->alignBoundsToObject();
	}
	//if currently added component is RigidBody, finalize it
	if (auto* casted = dynamic_cast<RigidBody*>(m_components.back().get()))
	{
		if (!getComponent<Collider>())
			throw std::runtime_error("Can't initialize RigidBody without collider");
		casted->finalizeRigidBody();
		RigidBodyRegistry::queueRigidBody(casted, 1);
	}
}

