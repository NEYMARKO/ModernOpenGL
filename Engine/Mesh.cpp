#include "Shader.h"
#include "Vertex.h"
#include "MeshLoader.h"
#include "Mesh.h"

Mesh::Mesh(Shader* boundingBoxShaderProgram, MeshLoader* meshLoader) : 
	m_id{ s_idGenerator++ }, mVAO{ VAO() }, mBoundingBoxShaderProgram{boundingBoxShaderProgram},
	boundingBox { BoundingBox(meshLoader->minExtremes, meshLoader->maxExtremes, this) }
{
	transferLoadedMeshInfo(meshLoader);
	setupBuffers();
}

Mesh::~Mesh()
{
	mVAO.Delete();
	mVBO.Delete();
	mEBO.Delete();

	std::cout << "DESTROYED MESH: " << m_id << std::endl;
}

void Mesh::setupBuffers()
{
	mVAO.Bind();
	mVBO = VBO(mVertices);
	mEBO = EBO(mIndices);

	mVAO.LinkVBO(mVBO, 0, 3, sizeof(Vertex), 0);
	mVAO.LinkVBO(mVBO, 1, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	mVAO.Unbind();
	mVBO.Unbind();
	mEBO.Unbind();
}

float Mesh::GetDistanceFromCamera()
{
	return this->mDistanceFromCamera;
}

int Mesh::GetID()
{
	return m_id;
}

void Mesh::transferLoadedMeshInfo(MeshLoader* meshLoader)
{
	mVertices = std::move(meshLoader->vertices);
	mIndices = std::move(meshLoader->indices);
	scalingFactor = meshLoader->scalingFactor;
}
