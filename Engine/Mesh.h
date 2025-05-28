#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Vertex;
class Shader;
class MeshLoader;

/// <summary>
/// Container for mesh data. Contains information about buffers, vertices and indices.
/// Vertex data is made of position and normal vectors.
/// </summary>
class Mesh
{
	private:
		static inline int s_idGenerator{};
		int m_id{};
		VBO mVBO;
		EBO mEBO;
		VAO mVAO;

		std::vector<Vertex> mVertices;
		std::vector<unsigned int> mIndices;

		void setupBuffers();

	public:
		glm::vec3 m_minimums;
		glm::vec3 m_maximums;

		float scalingFactor;

		Mesh(MeshLoader* meshLoader);
		~Mesh();
		void transferLoadedMeshInfo(MeshLoader* meshLoader);
		int GetID();
		float GetDistanceFromCamera();
		VAO* getVAO() { return &mVAO; }
		std::vector<Vertex>* getVertices() { return &mVertices; }
		std::vector<unsigned int>* getIndices() { return &mIndices; }


};