#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "VBO.h"
#include "VAO.h"

class Camera;
class Shader;
class EditorCollider;

class Gizmos
{
	private:
		Camera* m_camera;
		Shader* m_shader;
		VAO m_VAO;
		VBO m_VBO;
		std::vector<glm::vec3> m_edgePoints;
		std::vector<EditorCollider*> m_editorColliders;

		void expandBuffer();
		void repopulateVolumesPoints();
	public:
		Gizmos(Camera* camera, size_t bufferCapacity = 480 * sizeof(glm::vec3));
		~Gizmos() = default;
		void updateBufferContent();
		void renderBoundingVolumes();
		void addEditorCollider(EditorCollider* editorCollider);
};