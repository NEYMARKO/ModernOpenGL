#pragma once
#include "Shader.h"
#include "Camera.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

class MeshLoader
{
public:
	float scalingFactor = 1.0f;
	glm::vec3 minExtremes = glm::vec3(10000.0f, 10000.0f, 10000.0f);
	glm::vec3 maxExtremes = glm::vec3(-10000.0f, -10000.0f, -10000.0f);
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	MeshLoader(const char* filePath);
	void ReadFile(std::ifstream& file, std::vector<glm::vec3>& normalVectors);
	void SplitLine(std::string& line, std::vector<glm::vec3>& normalVectors);
	void CalculateScalingFactor();
	~MeshLoader();
};