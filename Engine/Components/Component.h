#pragma once
#include <typeinfo>
#include <typeindex>
class SceneEntity;

/// <summary>
/// Able to attach to Object and render in GUI 
/// </summary>
class Component
{
private:
	SceneEntity* m_parentObject = nullptr;
public:
	Component() = default;
	virtual ~Component() = default;
	void setParentObject(SceneEntity* parent) { m_parentObject = parent; };
	SceneEntity* getParentObject() const { return m_parentObject; }
	virtual void* getBase(std::type_index t) = 0;
};