#pragma once
#include <memory>
#include <string>
#include <vector>

class Transform;
class MeshRenderer;

class Object
{
private:

	std::unique_ptr<Transform> mTransform;
	std::unique_ptr<MeshRenderer> mMeshRenderer;
	std::vector<std::unique_ptr<Object>> mChildren;
	Object* mParentObject;
	std::string mName;

public:
	Object(std::unique_ptr<Transform> transform, std::unique_ptr<MeshRenderer> meshRenderer, Object* parent = nullptr);

	void setName(const std::string& name) { mName = name; }
	void setParent(Object* parent) { mParentObject = parent; }
	void removeParent() { mParentObject = nullptr; }
	Object* getParent() const { return mParentObject; }

	std::string getName() const { return mName; }
	
	template<typename T> void addComponent(std::unique_ptr<T> component) const;
	template<typename T> T* getComponent();

};