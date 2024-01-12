#pragma once
#include "Shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
class Mesh;


struct Plane
{
	glm::vec3 normal;
	float D;
};
class BoundingBox
{
	private:
		VBO boxVBO;
		EBO boxEBO;
		VAO boxVAO;
		Mesh& parentMesh;
		glm::vec3 minExtremes;
		glm::vec3 maxExtremes;
	public:
		//std::vector<float> bounds;
		std::vector<Plane> planeCoefficients;
		//After filling VBO with info, vertices are converted to world space because they are no longer needed in object space
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		//Constructor for BoundingBox 
		//Uses extremes of Mesh to create box which contains all vertices of Mesh object
		BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh& parentMesh);
		~BoundingBox();
		
		//Initializes borders, fills vertices and indices with data 
		void Initialize();
		//Sets up VAO, VBO and EBO buffers
		void SetupBuffers();
		//Translates box bounds (extremes) to world space and checks intersection with ray in world space
		void VerticesToWorld();
		//Checks if ray intersects with bounding box in world space
		bool Intersects(Camera& camera, float step);
		//Used for updating BoundingBox bounds when Mesh is transformed
		void UpdateBounds(const glm::mat4& transformationMatrix);
		//Draws BoundingBox around Mesh
		void Draw(Shader& shaderProgram, Camera& camera);
};