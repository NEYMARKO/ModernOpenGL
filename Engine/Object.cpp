#include "Transform.h"
#include "MeshRenderer.h"
#include "Object.h"

Object::Object(std::unique_ptr<Transform> transform, std::unique_ptr<MeshRenderer> meshRenderer, Object* parent)
	: mTransform{ std::move(transform) }, mMeshRenderer{ std::move(meshRenderer) }, mParentObject{ parent }
{
	mTransform.get()->setParent(this);
	mMeshRenderer.get()->setParent(this);
}


template <typename T>
void Object::addComponent(std::unique_ptr<T> component) const
{
	if (std::is_same<T, Transform>::value)
		mTransform = std::move(component);
	else if (std::is_same<T, MeshRenderer>::value)
		mMeshRenderer = std::move(component);
	else
		throw std::runtime_error("Unsupported component type")
}

template<typename T> 
T* Object::getComponent()
{
	if (std::is_same<T, Transform>::value) 
		return mTransform.get();
	else if (std::is_same<T, MeshRenderer>::value) 
		return mMeshRenderer.get();
	else return nullptr;
}