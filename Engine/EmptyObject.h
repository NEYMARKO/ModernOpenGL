#pragma once
#include <memory>
#include <string_view>
#include <vector>

class Transform;

class EmptyObject
{
private:
	std::unique_ptr<Transform> m_transform;
	//if parent gets destroyed, it's children should get destroyed aswell
	std::vector<std::unique_ptr<EmptyObject>> m_children;
public:
	std::string_view m_name;
	EmptyObject* m_parent;
};