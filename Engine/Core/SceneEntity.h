#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>
#include <memory>
#include "../Components/Component.h"
#include "../Components/Transform.h"
#include "../Components/EditorCollider.h"

struct SceneEntity
{
	std::string m_name;
	Transform m_transform;
	EditorCollider m_editorCollider;
	SceneEntity* m_parent;
	std::vector<SceneEntity*> m_children;

	SceneEntity(EditorCollider&& editorCollider, Transform&& transform, const std::string& name = "Object", SceneEntity* parentEntity = nullptr) :
		m_editorCollider{ std::move(editorCollider) }, 
		m_transform{ std::move(transform) },
		m_parent{ parentEntity }, m_name { name }
	{
	}

	template <typename T>
	T* getComponent() {
		return reinterpret_cast<T*>(getComponentLow(typeid(T)));
	}

	virtual void* getComponentLow(std::type_index t)
	{
		if (t == typeid(Transform))
			return &m_transform;
		return nullptr;
	}

	virtual void addComponent(std::unique_ptr<Component> component)
	{
		if (!component)
			throw std::runtime_error("Component is null");
	}

	void changeName(const std::string& name) { m_name = name; }

	virtual ~SceneEntity() {};
};