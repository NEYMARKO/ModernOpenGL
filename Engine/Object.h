#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

#include "Transform.h"
#include "MeshRenderer.h"

class Component;

class Object
{
private:

	Transform m_transform;
	MeshRenderer m_meshRenderer;
	std::vector<std::unique_ptr<Component>> m_components;
	std::vector<std::unique_ptr<Object>> mChildren;
	Object* m_parentObject;
	std::string m_name;

public:
	Object(Transform&& transform, MeshRenderer&& meshRenderer, 
		const std::string& name = "object", Object* parent = nullptr);

	void setName(const std::string& name) { m_name = name; }
	void setParent(Object* parent) { m_parentObject = parent; }
	void removeParent() { m_parentObject = nullptr; }
	Object* getParent() const { return m_parentObject; }
	std::string getName() const { return m_name; }

	void addComponent(std::unique_ptr<Component> component);

	template<typename T>
	T* getComponent()
	{
		if constexpr (std::is_same<T, Transform>::value)
			return &m_transform;
		else if constexpr (std::is_same<T, MeshRenderer>::value)
			return &m_meshRenderer;

		for (auto& component : m_components)
		{
			if (auto* casted = dynamic_cast<T*>(component.get()))
			{
				return casted;
			}
		}
		return nullptr;
	}

};