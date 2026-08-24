#pragma once
#include <string_view>
#include <memory>
#include <unordered_map>

/// <summary>
/// Manager for resources (meshes, shaders, materials). Holds information about all created
/// resources and gives access to them (through pointers). 
/// </summary>
template<typename T>
class ResourceManager
{
private:
	static std::unordered_map<std::string_view, std::unique_ptr<T>> m_resources;
public:
	ResourceManager() = default;
	~ResourceManager() { m_resources.clear(); };
	
	//Adds resource if it doesn't already exist. Also returns pointer to the resource
	//after it has been moved, so that Object could use it - it only needs pointer/reference
	//to the existing resource, not the resource itself
	static T* addResource(std::string_view name, std::unique_ptr<T> resource)
	{
		//resource already exists
		if (m_resources.find(name) != m_resources.end())
			return m_resources[name].get();
		else
		{
			m_resources[name] = std::move(resource);
			return m_resources[name].get();
		}
	}

	static T* getResource(std::string_view name)
	{
		auto it = m_resources.find(name);
		if (it != m_resources.end())
			return it->second.get();
		else
			return nullptr;
	}
};

//m_resources is static and needs to be defined outside of the class
//since it is static, three is only one instance of it for all ResourceManager<T> instances
//(it can't be redefined)
template<typename T>
std::unordered_map<std::string_view, std::unique_ptr<T>> ResourceManager<T>::m_resources;