#pragma once
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "MeshLoader.h"
#include "BoundingBox.h"
#include <memory>

class Shader;
//class Lighting;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

class Mesh
{
	private:
		static inline int s_idGenerator{};
		int m_id{};
		std::unique_ptr<VBO> mVBO;
		std::unique_ptr<EBO> mEBO;
		std::unique_ptr<VAO> mVAO;

		glm::vec3 color = glm::vec3(0.862745f, 0.862745f, 0.862745f);

		std::vector<Vertex> mVertices;
		std::vector<unsigned int> mIndices;

		/*glm::mat4 mTranslationMatrix = glm::mat4(1.0f);
		glm::mat4 mRotationMatrix = glm::mat4(1.0f);
		glm::mat4 mScalingMatrix = glm::mat4(1.0f);
		glm::mat4 mFinalMatrix = glm::mat4(1.0f);*/
		//Binds buffers, fills VAO & EBO with data, in the end unbinds buffers

		Shader* mShaderProgram;
		Shader* mBoundingBoxShaderProgram;
		float mDistanceFromCamera;

		void setupMesh();
	public:
		std::unique_ptr<BoundingBox> boundingBox;
		//BoundingBox* boundingBox;
		//MeshLoader* mMeshLoader;

		//glm::vec3 objectPos = glm::vec3(0.0, 0.0, 0.0);
		float scalingFactor;
		//VAO mVAO;
		
		Mesh(Shader* shaderProgram, Shader* boundingBoxShaderProgram, MeshLoader* meshLoader, glm::vec3 objectPos);
		~Mesh();
		void ChangeColor(const glm::vec3& color);
		//void InitialTransform(glm::vec3 translation, float scale);
		//void Translate(const glm::vec3& newPosition);
		//void Rotate(const glm::vec3& rotationVector, float angle);
		//void Rotate(const glm::quat& rotation);
		//void Scale(float scale);
		//void CalculateFinalMatrix();
		//void CalculateDistanceFromCamera(Camera* camera);
		float GetDistanceFromCamera();
		int GetID();
		//glm::vec3 GetPosition();
		//glm::mat4 GetFinalMatrix();
		//Binds VAO, calculates MVP matrices, assigns uniforms and draws object using EBO info
		//void Render(Camera& camera, Lighting& lighting);

		void transferLoadedMeshInfo(MeshLoader* meshLoader);
		VAO* getVAO() { return mVAO.get(); }
};