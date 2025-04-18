#pragma once

class Object;

class Component
{
private:
	Object* mParentObject;
public:
	Component(Object* parentObject) : mParentObject(parentObject) {}
	virtual ~Component() = default;
	Object* getParentObject() const { return mParentObject; }
};