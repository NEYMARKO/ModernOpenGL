#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "ResourceManager.h"

std::unordered_map<std::string_view, std::unique_ptr<Mesh>> ResourceManager::m_meshes;
std::unordered_map<std::string_view, std::unique_ptr<Shader>> ResourceManager::m_shaders;
std::unordered_map<std::string_view, std::unique_ptr<Material>> ResourceManager::m_materials;

ResourceManager::~ResourceManager()
{
	m_meshes.clear();
	m_materials.clear();
	//Shaders should be deleted after Materials since materials use them as
	//member variables
	m_shaders.clear();
}