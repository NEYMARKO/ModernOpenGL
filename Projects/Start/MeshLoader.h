#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>
#include "Mesh.h"

class Mesh;

class MeshLoader
{
	private:
		glm::vec3 minExtremes = glm::vec3(10000.0f, 10000.0f, 10000.0f);
		glm::vec3 maxExtremes = glm::vec3(-10000.0f, -10000.0f, -10000.0f);
	public:
		float scalingFactor;
		MeshLoader(const char* filePath, Mesh* mesh);
		void ReadFile(std::ifstream& file, Mesh* mesh, std::vector<glm::vec3>& normalVectors);
		void SplitLine(std::string& line, Mesh* mesh, std::vector<glm::vec3>& normalVectors);
		void NormalizeObject(Mesh* mesh);
		~MeshLoader();
};