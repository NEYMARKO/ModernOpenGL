#pragma once

class Object;

class Component
{
private:
	Object* m_parentObject = nullptr;
public:
	Component() = default;
	virtual ~Component() = default;
	void setParentObject(Object* parent) { m_parentObject = parent; };
	Object* getParentObject() const { return m_parentObject; }
};