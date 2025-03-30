#include "Mesh.h"
#include "Shader.h"
#include "Lighting.h"

Mesh::Mesh(Shader* shaderProgram, Shader* boundingBoxShaderProgram, MeshLoader* meshLoader, glm::vec3 objectPosition, float id)
{
	mShaderProgram = shaderProgram;
	mBoundingBoxShaderProgram = boundingBoxShaderProgram;
	this->meshLoader = meshLoader;
	this->id = id;
	setupMesh();
	InitialTransform(objectPosition, this->meshLoader->scalingFactor);
	boundingBox = std::make_unique<BoundingBox>(meshLoader->minExtremes, meshLoader->maxExtremes, this);
	//boundingBox = new BoundingBox(meshLoader->minExtremes, meshLoader->maxExtremes, this);

	//std::cout << "Spawned mesh with id: " << id << std::endl;
}

Mesh::~Mesh()
{
	mVAO.Delete();
	mVBO.Delete();
	mEBO.Delete();

	std::cout << "DESTROYED MESH: " << id << std::endl;
}

void Mesh::setupMesh()
{
	this->mVAO.Bind();
	this->mVBO = VBO(this->meshLoader->vertices);
	this->mEBO = EBO(this->meshLoader->indices);

	this->mVAO.LinkVBO(mVBO, 0, 3, sizeof(Vertex), 0);
	this->mVAO.LinkVBO(mVBO, 1, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//this->mVAO.LinkVBO(mVBO, 2, 3, sizeof(Vertex), (void*)offsetof(Vertex, color));

	this->mVAO.Unbind();
	this->mVBO.Unbind();
	this->mEBO.Unbind();

}

void Mesh::ChangeColor(const glm::vec3& color)
{
	this->color = color;
}

void Mesh::InitialTransform(glm::vec3 translation, float scale)
{
	this->objectPos = translation;
	this->mTranslationMatrix = glm::translate(this->mTranslationMatrix, translation);
	this->mScalingMatrix = glm::scale(this->mScalingMatrix, glm::vec3(scale, scale, scale));
	CalculateFinalMatrix();
}

void Mesh::Translate(const glm::vec3& newPosition)
{
	this->objectPos = newPosition;
	this->mTranslationMatrix = glm::translate(glm::mat4(1.0), newPosition);
	CalculateFinalMatrix();
	this->boundingBox->VerticesToWorld();
}
void Mesh::Rotate(const glm::vec3& rotationVector, float angle)
{
	this->mRotationMatrix = glm::rotate(this->mRotationMatrix, glm::radians(angle), rotationVector);
	CalculateFinalMatrix();
	this->boundingBox->VerticesToWorld();

}
void Mesh::Rotate(const glm::quat& rotation)
{
	//std::cout << "CHANGED ROTATION" << std::endl;
	this->mRotationMatrix = glm::toMat4(rotation);
	CalculateFinalMatrix();
	this->boundingBox->VerticesToWorld();
}
void Mesh::Scale(float scale)
{
	this->mScalingMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scale, scale, scale));
	CalculateFinalMatrix();
	this->boundingBox->VerticesToWorld();

}
void Mesh::CalculateFinalMatrix()
{
	this->mFinalMatrix = this->mTranslationMatrix * this->mRotationMatrix * this->mScalingMatrix;
}

glm::vec3 Mesh::GetPosition()
{
	return glm::vec3(this->mFinalMatrix[3][0], this->mFinalMatrix[3][1], this->mFinalMatrix[3][2]);
}
glm::mat4 Mesh::GetFinalMatrix()
{
	return this->mFinalMatrix;
}

void Mesh::CalculateDistanceFromCamera(Camera* camera)
{
	this->distanceFromCamera = glm::distance(this->objectPos, camera->GetCameraPosition());
}

float Mesh::GetDistanceFromCamera()
{
	return this->distanceFromCamera;
}

void Mesh::Render(Camera& camera, Lighting& lighting)
{
	mShaderProgram->Activate();

	mShaderProgram->SetMat4("model", this->mFinalMatrix);
	camera.ViewProjectionMatrix(*mShaderProgram);

	mShaderProgram->SetVec3("objectColor", this->color);
	mShaderProgram->SetVec3("lightColor", lighting.getColor());
	mShaderProgram->SetVec3("lightPos", lighting.getPosition());

	mVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshLoader->indices.size(), GL_UNSIGNED_INT, 0);
	mVAO.Unbind();
}