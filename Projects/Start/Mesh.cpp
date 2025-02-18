#include "Mesh.h"
#include "Lighting.h"


Mesh::Mesh()
{

}
Mesh::Mesh(MeshLoader* meshLoader, glm::vec3 objectPosition, float id)
{
	this->meshLoader = meshLoader;
	this->id = id;
	setupMesh();
	InitialTransform(objectPosition, this->meshLoader->scalingFactor);
	this->boundingBox = new BoundingBox(meshLoader->minExtremes, meshLoader->maxExtremes, *this);

	//std::cout << "Spawned mesh with id: " << id << std::endl;
}

Mesh::~Mesh()
{
	mVAO.Delete();
	mVBO.Delete();
	mEBO.Delete();

	if (this->boundingBox != nullptr) {
		delete this->boundingBox;
		this->boundingBox = nullptr;  
	}

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
	this->translationMatrix = glm::translate(this->translationMatrix, translation);
	this->scalingMatrix = glm::scale(this->scalingMatrix, glm::vec3(scale, scale, scale));
	CalculateFinalMatrix();
}

void Mesh::Translate(const glm::vec3& newPosition)
{
	this->objectPos = newPosition;
	this->translationMatrix = glm::translate(glm::mat4(1.0), newPosition);
	CalculateFinalMatrix();
	this->boundingBox->VerticesToWorld();
}
void Mesh::Rotate(const glm::vec3& rotationVector, float angle)
{
	this->rotationMatrix = glm::rotate(this->rotationMatrix, glm::radians(angle), rotationVector);
	CalculateFinalMatrix();
	this->boundingBox->VerticesToWorld();

}
void Mesh::Rotate(const glm::quat& rotation)
{
	//std::cout << "CHANGED ROTATION" << std::endl;
	this->rotationMatrix = glm::toMat4(rotation);
	CalculateFinalMatrix();
	this->boundingBox->VerticesToWorld();
}
void Mesh::Scale(float scale)
{
	this->scalingMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scale, scale, scale));
	CalculateFinalMatrix();
	this->boundingBox->VerticesToWorld();

}
void Mesh::CalculateFinalMatrix()
{
	this->finalMatrix = this->translationMatrix * this->rotationMatrix * this->scalingMatrix;
}

glm::vec3 Mesh::GetPosition()
{
	return glm::vec3(this->finalMatrix[3][0], this->finalMatrix[3][1], this->finalMatrix[3][2]);
}
glm::mat4 Mesh::GetFinalMatrix()
{
	return this->finalMatrix;
}

void Mesh::CalculateDistanceFromCamera(Camera* camera)
{
	this->distanceFromCamera = glm::distance(this->objectPos, camera->GetCameraPosition());
}

float Mesh::GetDistanceFromCamera()
{
	return this->distanceFromCamera;
}

void Mesh::Render(Shader& shaderProgram, Shader& boundingBoxShaderProgram, Camera& camera, Lighting& lighting)
{
	shaderProgram.Activate();

	shaderProgram.SetMat4("model", this->finalMatrix);
	camera.ViewProjectionMatrix(shaderProgram);

	shaderProgram.SetVec3("objectColor", this->color);
	shaderProgram.SetVec3("lightColor", lighting.lightColor);
	shaderProgram.SetVec3("lightPos", lighting.position);

	mVAO.Bind();
	glDrawElements(GL_TRIANGLES, this->meshLoader->indices.size(), GL_UNSIGNED_INT, 0);
	mVAO.Unbind();
}