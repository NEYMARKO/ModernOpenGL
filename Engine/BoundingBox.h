#pragma once
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
//#include "Camera.h"
#include "glm/glm.hpp"

class Shader;
class Camera;
class Mesh;

class BoundingBox
{
	private:
		VBO mBoxVBO;
		EBO mBoxEBO;
		VAO mBoxVAO;
		glm::vec3 mMinExtremes;
		glm::vec3 mMaxExtremes;
		Mesh* mParentMesh;
		//needed to be able to calculate world min and max extremes
		glm::vec3 mLocalMinExtremes;
		glm::vec3 mLocalMaxExtremes;
		std::vector<glm::vec3> mVertices;
		std::vector<unsigned int> mIndices;
	public:
		glm::vec3 boxCenter;
		BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh* parentMesh);
		~BoundingBox();
		//Initializes borders, fills vertices and indices with data 
		void Initialize();
		//Initializes buffers, fills VBO & EBO with data, in the end unbinds buffers
		void SetupBuffers();
		//Translates box bounds (extremes) to world space and checks intersection with ray in world space
		void VerticesToWorld();
		//Checks if ray intersects with bounding box in world space
		bool Intersects(Camera& camera, float step) const;
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