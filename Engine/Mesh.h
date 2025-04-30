#pragma once
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "MeshLoader.h"
#include "BoundingBox.h"
#include "Vertex.h"
#include <memory>

class Shader;
//class Lighting;

/// <summary>
/// Container for mesh data. Contains information about buffers, vertices and indices.
/// Vertex data is made of position and normal vectors.
/// </summary>
class Mesh
{
	private:
		static inline int s_idGenerator{};
		int m_id{};
		std::unique_ptr<VBO> mVBO;
		std::unique_ptr<EBO> mEBO;
		std::unique_ptr<VAO> mVAO;

		std::vector<Vertex> mVertices;
		std::vector<unsigned int> mIndices;

		Shader* mBoundingBoxShaderProgram;
		float mDistanceFromCamera;

		void setupBuffers();

	public:
		std::unique_ptr<BoundingBox> boundingBox;
		float scalingFactor;

		Mesh(Shader* boundingBoxShaderProgram, MeshLoader* meshLoader);
		~Mesh();
		int GetID();
		float GetDistanceFromCamera();
		VAO* getVAO() { return mVAO.get(); }
		std::vector<Vertex>* getVertices() { return &mVertices; }
		std::vector<unsigned int>* getIndices() { return &mIndices; }
		
		//void ChangeColor(const glm::vec3& color);

		void transferLoadedMeshInfo(MeshLoader* meshLoader);

};