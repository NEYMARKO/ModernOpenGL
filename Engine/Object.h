#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

#include "SceneEntity.h"
#include "MeshRenderer.h"


class Component;

class Object : public SceneEntity
{
private:
	MeshRenderer m_meshRenderer;
	std::vector<std::unique_ptr<Component>> m_components;

public:

	Object(Transform&& transform, MeshRenderer&& meshRenderer, 
		const std::string& name = "object", SceneEntity* parent = nullptr);

	/*void setName(const std::string& name) { m_name = name; }
	void setParent(Object* parent) { m_parentObject = parent; }
	void removeParent() { m_parentObject = nullptr; }
	Object* getParent() const { return m_parentObject; }*/
	std::string getName() const { return m_name; }
	EditorCollider* getEditorCollider() { return &m_editorCollider; };
	void addComponent(std::unique_ptr<Component> component);

	virtual void* getComponentLow(std::type_index t) override
	{
		if (t == typeid(Transform))
			return &m_transform;
		if (t == typeid(MeshRenderer))
			return &m_meshRenderer;
		for (auto& c : m_components)
		{
			if (c->getBase(t) != nullptr)
				return c.get();
		}
		return nullptr;
	}

};