#pragma once

#include <memory>
#include <vector>
#include <array>
#include <glm/glm.hpp>

#include "../Rendering/Buffers/VAO.h"
#include "../Rendering/Buffers/VBO.h"
#include "../Rendering/Buffers/EBO.h"

struct Vertex;
struct VertexTex;
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
		std::array<VertexTex, 4> m_verticesTex;
		std::array<unsigned int, 6> m_indicesTex;

		void setupBuffers();
		void setupBuffersForTexture();

	public:
		glm::vec3 m_minimums;
		glm::vec3 m_maximums;

		float scalingFactor;

		Mesh(std::array<VertexTex, 4>&& vertices,
			std::array<unsigned int, 6>&& indices) :
			m_verticesTex{ std::move(vertices) }, m_indicesTex{ std::move(indices) }
		{
			setupBuffersForTexture();
		}
		Mesh(MeshLoader* meshLoader);
		~Mesh();
		void transferLoadedMeshInfo(MeshLoader* meshLoader);
		int GetID();
		VAO* getVAO() { return &mVAO; }
		std::vector<Vertex>* getVertices() { return &mVertices; }
		std::vector<unsigned int>* getIndices() { return &mIndices; }
};