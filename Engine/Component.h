#pragma once

class Object;

class Component
{
private:
	Object* mParentObject = nullptr;
public:
	Component() = default;
	//Component(Object* parentObject) : mParentObject(parentObject) {}
	virtual ~Component() = default;
	void setParentObject(Object* parent) { mParentObject = parent; };
	Object* getParentObject() const { return mParentObject; }
};