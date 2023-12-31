#pragma once
#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <algorithm>

class Mesh;

class MeshLoader
{
	public:
		float scalingFactor;
		glm::vec3 minExtremes = glm::vec3(10000.0f, 10000.0f, 10000.0f);
		glm::vec3 maxExtremes = glm::vec3(-10000.0f, -10000.0f, -10000.0f);
		MeshLoader(const char* filePath, Mesh* mesh);
		void ReadFile(std::ifstream& file, Mesh* mesh, std::vector<glm::vec3>& normalVectors);
		void SplitLine(std::string& line, Mesh* mesh, std::vector<glm::vec3>& normalVectors);
		void NormalizeObject(Mesh* mesh);
		~MeshLoader();
};