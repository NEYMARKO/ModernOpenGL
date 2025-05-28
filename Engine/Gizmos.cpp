#include "Camera.h"
#include "Shader.h"
#include "EditorCollider.h"
#include "ResourceManager.h"
#include "Gizmos.h"

Gizmos::Gizmos(Camera* camera, size_t bufferCapacity) :
	m_camera{ camera }, 
	m_VBO{ bufferCapacity }
{
	m_edgePoints.reserve(24 * 30 * sizeof(glm::vec3));
	
	m_VAO.Bind();
	m_VAO.LinkVBO(m_VBO, 0, 3, sizeof(glm::vec3), 0);

	m_VAO.Unbind();
	m_VBO.Unbind();
}

void Gizmos::addEditorCollider(EditorCollider* editorCollider)
{
	m_editorColliders.emplace_back(editorCollider);
	updateBufferContent();
}
//TODO: somehow connect points to bounding volume they belong to - it doesn't make sense
//to update whole buffer if only one bounding volume is changed - just update the part of the
//buffer in which those changed points are stored in
void Gizmos::updateBufferContent()
{
	repopulateVolumesPoints();

	m_VBO.Bind();
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, m_edgePoints.data(),
		m_edgePoints.size() * sizeof(glm::vec3));
	glUnmapBuffer(GL_ARRAY_BUFFER);
	m_VBO.Unbind();
}

void Gizmos::repopulateVolumesPoints()
{
	m_edgePoints.clear();
	for (auto* eCollider : m_editorColliders)
	{
		m_edgePoints.insert(m_edgePoints.end(), 
			eCollider->m_edges.begin(), eCollider->m_edges.end());
	}
	//current number of points is greater than what VBO can store (not enough allocated VBO storage)
	if ((m_VBO.m_storageCapacity / sizeof(glm::vec3)) < m_edgePoints.size())
		expandBuffer();
}

void Gizmos::expandBuffer()
{
	size_t currentCapacity = m_VBO.m_storageCapacity;
	
	m_VBO.Delete();
	m_VBO = VBO(2 * currentCapacity);
	
	m_VAO.Bind();
	m_VAO.LinkVBO(m_VBO, 0, 3, sizeof(glm::vec3), 0);
	m_VAO.Unbind();

	m_VBO.Unbind();
}

void Gizmos::renderBoundingVolumes()
{
	if (!m_shader)
		m_shader = ResourceManager<Shader>::getResource("gizmos");
	m_shader->Activate();
	m_camera->generateViewProjectionMatrices(*m_shader);

	m_VAO.Bind();
	glDrawArrays(GL_LINES, 0, m_edgePoints.size());
	m_VAO.Unbind();
}