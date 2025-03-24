#pragma once
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
class Mesh;

class BoundingBox
{
	private:
		VBO boxVBO;
		EBO boxEBO;
		VAO boxVAO;
		Mesh& parentMesh;
		glm::vec3 minExtremes;
		glm::vec3 maxExtremes;
		//needed to be able to calculate world min and max extremes
		glm::vec3 localMinExtremes;
		glm::vec3 localMaxExtremes;
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
	public:
		glm::vec3 boxCenter;
		BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh);
		~BoundingBox();
		//Initializes borders, fills vertices and indices with data 
		void Initialize();
		//Initializes buffers, fills VBO & EBO with data, in the end unbinds buffers
		void SetupBuffers();
		//Translates box bounds (extremes) to world space and checks intersection with ray in world space
		void VerticesToWorld();
		//Checks if ray intersects with bounding box in world space
		bool Intersects(Camera& camera, float step);
		//Used for updating BoundingBox bounds when Mesh is transformed
		void UpdateBounds(const glm::mat4& transformationMatrix);
		//Draws BoundingBox around Mesh
		void Draw(Shader& shaderProgram, Camera& camera);

		glm::vec3 GetMinExtremes();
		glm::vec3 GetMaxExtremes();

		VAO* GetBoundingBoxVAO();
		Mesh* GetParentMesh();
		std::vector<unsigned int>* GetIndices();
};