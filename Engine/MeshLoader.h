#pragma once
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <memory>

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