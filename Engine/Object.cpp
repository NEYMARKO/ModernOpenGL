#include "Transform.h"
#include "MeshRenderer.h"
#include "Component.h"
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
}

