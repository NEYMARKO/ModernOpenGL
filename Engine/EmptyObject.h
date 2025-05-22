#pragma once
#include <string_view>
#include <vector>

#include "Transform.h"

class EmptyObject
{
private:
	Transform m_transform;
	//if parent gets destroyed, it's children should get destroyed as well
	std::vector<std::unique_ptr<EmptyObject>> m_children;
public:
	std::string_view m_name;
	EmptyObject* m_parent;
};