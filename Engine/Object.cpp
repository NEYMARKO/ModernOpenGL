#include "Transform.h"
#include "MeshRenderer.h"
#include "Object.h"

Object::Object(std::unique_ptr<Transform> transform, std::unique_ptr<MeshRenderer> meshRenderer, Object* parent)
	: mTransform{ std::move(transform) }, mMeshRenderer{ std::move(meshRenderer) }, mParentObject{ parent }
{
	mTransform.get()->setParent(this);
	mMeshRenderer.get()->setParent(this);
	mTransform.get()->setScale(mMeshRenderer.get()->getMesh()->scalingFactor);
}



