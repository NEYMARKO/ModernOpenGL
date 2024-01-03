#pragma once
#include "Shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
class Mesh;

class BoundingBox
{
	private:
		Mesh& parentMesh;
		VBO boxVBO;
		EBO boxEBO;
		VAO boxVAO;
	public:
		std::vector<float> bounds;
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		//Constructor for BoundingBox 
		//Uses extremes of Mesh to create box which contains all vertices of Mesh object
		BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh);
		~BoundingBox();
		void Initialize();

		void SetupBuffers();
		//Used for updating BoundingBox bounds when Mesh is transformed
		void UpdateScale(float scale);

		void UpdateRotation(glm::vec3 rotationAxis, float angle);
		//Draws BoundingBox around Mesh
		void Draw(Shader& shaderProgram, Camera& camera);
};