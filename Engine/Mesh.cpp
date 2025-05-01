#include "Mesh.h"
#include "Shader.h"
//#include "Lighting.h"

Mesh::Mesh(Shader* boundingBoxShaderProgram, MeshLoader* meshLoader) : 
	m_id{ s_idGenerator++ }, mVAO{ std::make_unique<VAO>() }, mBoundingBoxShaderProgram{boundingBoxShaderProgram}
{
	transferLoadedMeshInfo(meshLoader);
	setupBuffers();
	boundingBox = std::make_unique<BoundingBox>(meshLoader->minExtremes, meshLoader->maxExtremes, this);
}

Mesh::~Mesh()
{
	mVAO.get()->Delete();
	mVBO.get()->Delete();
	mEBO.get()->Delete();

	std::cout << "DESTROYED MESH: " << m_id << std::endl;
}

void Mesh::setupBuffers()
{
	mVAO->Bind();
	mVBO = std::make_unique<VBO>(mVertices);
	mEBO = std::make_unique<EBO>(mIndices);

	mVAO->LinkVBO(*(mVBO.get()), 0, 3, sizeof(Vertex), 0);
	mVAO->LinkVBO(*(mVBO.get()), 1, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	mVAO->Unbind();
	mVBO->Unbind();
	mEBO->Unbind();
}

//void Mesh::ChangeColor(const glm::vec3& color)
//{
//	this->color = color;
//}

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
