#pragma once
#include <string_view>
#include <memory>
#include <unordered_map>

class Mesh;
class Shader;
class Material;

/// <summary>
/// Manager for resources (meshes, shaders, materials). Holds information about all created
/// resources and gives access to them (through pointers). 
/// </summary>
class ResourceManager
{
private:
	static std::unordered_map<std::string_view, std::unique_ptr<Mesh>> m_meshes;
	static std::unordered_map<std::string_view, std::unique_ptr<Shader>> m_shaders;
	static std::unordered_map<std::string_view, std::unique_ptr<Material>> m_materials;
public:
	ResourceManager() = default;
	~ResourceManager();
	
	//Adds resource if it doesn't already exist. Also returns pointer to the resource
	//after it has been moved, so that Object could use it - it only needs pointer/reference
	//to the existing resource, not the resource itself
	template<typename T>
	static T* addResource(const std::string_view name, std::unique_ptr<T> resource)
	{
		if (alreadyExists<T>(name))
			return;
		if constexpr (auto* casted = dynamic_cast<Mesh*>(resource.get()))
		{
			m_meshes[name] = std::move(resource);
			return m_meshes[name].get();
		}
		else if constexpr (auto* casted = dynamic_cast<Shader*>(resource.get()))	
		{ 
			m_shaders[name] = std::move(resource);
			return nullptr;
		}
		else if constexpr (auto* casted = dynamic_cast<Material*>(resource.get()))
		{
			m_materials[name] = std::move(resource);
			return m_materials[name].get();
		}
		else
			throw std::runtime_error("Resource type is not supported");
	}

	template<typename T>
	bool alreadyExists(const std::string_view name) const 
	{
		if constexpr (std::is_same<T, Mesh*>())
			return m_meshes.find(name) != m_meshes.end();
		else if constexpr (std::is_same<T, Shader*>())
			return m_shaders.find(name) != m_shaders.end();
		else if constexpr (std::is_same<T, Material*>())
			return m_materials.find(name) != m_materials.end();
		else
			throw std::runtime_error("Resource type is not supported");
	}

	template<typename T>
	static T* getResource(const std::string_view name)
	{
		if constexpr (auto* casted = dynamic_cast<Mesh>(T))
		{
			auto it = m_meshes.find(name);
			if (it != m_meshes.end())
				return it->second.get();
		}
		else if constexpr (auto* casted = dynamic_cast<Shader>(T))
		{
			auto it = m_shaders.find(name);
			if (it != m_shaders.end())
				return it->second.get();
		}
		else if constexpr (auto* casted = dynamic_cast<Material>(T))
		{
			auto it = m_materials.find(name);
			if (it != m_materials.end())
				return it->second.get();
		}
		else
			throw std::runtime_error("Resource type is not supported");
	}
};