#include "Shader.h"
#include "Vertex.h"
#include "MeshLoader.h"
#include "Mesh.h"

Mesh::Mesh(MeshLoader* meshLoader) : 
	m_id{ s_idGenerator++ }, mVAO{ VAO() }
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

int Mesh::GetID()
{
	return m_id;
}

void Mesh::transferLoadedMeshInfo(MeshLoader* meshLoader)
{
	mVertices = std::move(meshLoader->vertices);
	mIndices = std::move(meshLoader->indices);
	scalingFactor = meshLoader->scalingFactor;

	m_minimums = meshLoader->minExtremes;
	m_maximums = meshLoader->maxExtremes;
}
