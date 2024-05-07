#include "Gizmos.h"

Gizmos::Gizmos(Camera* camera, Shader* meshShader, Shader* boundingBoxShader)
{
	this->camera = camera;
	this->boundingBoxShader = boundingBoxShader;
	this->meshShader = meshShader;
}

void Gizmos::AddRay(std::string name, const glm::vec3& startPos, const glm::vec3& direction, int length)
{
	Ray* newRay = new Ray(startPos, direction, length);
	this->raysInTheScene[name] = newRay;
}

//position is getting fetched from world space => no need for converting
void Gizmos::RenderLine(Ray* ray, glm::vec3& color)
{
	this->boundingBoxShader->Activate();

	glLineWidth(4.0f);
	//Coordinates are already converted to world space in Raycast function
	glm::mat4 model = glm::mat4(1.0f);

	this->boundingBoxShader->SetMat4("model", model);
	this->camera->ViewProjectionMatrix(*(this->boundingBoxShader));

	this->boundingBoxShader->SetVec3("lineColor", color);

	ray->GetRayVAO()->Bind();
	glDrawElements(GL_LINES, ray->GetRayIndices()->size(), GL_UNSIGNED_INT, 0);
	ray->GetRayVAO()->Bind();
}

void Gizmos::RenderAllLines(glm::vec3& color)
{
	for (auto ray : this->raysInTheScene)
	{
		RenderLine(ray.second, color);
	}
}

void Gizmos::UpdateLine(std::string& name, const glm::vec3& startPos, const glm::vec3& direction, int length)
{
	this->raysInTheScene[name] = new Ray(startPos, direction, length);
}
void Gizmos::RenderBoundingBox(BoundingBox* boundingBox)
{
	this->boundingBoxShader->Activate();

	glm::mat4 model = boundingBox->GetParentMesh()->GetFinalMatrix();
	this->boundingBoxShader->SetMat4("model", model);
	this->boundingBoxShader->SetVec3("cameraPos", this->camera->GetCameraPosition());
	this->boundingBoxShader->SetVec3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));

	this->camera->ViewProjectionMatrix(*(this->boundingBoxShader));

	boundingBox->GetBoundingBoxVAO()->Bind();

	glDrawElements(GL_LINES, boundingBox->GetIndices()->size(), GL_UNSIGNED_INT, 0);

	boundingBox->GetBoundingBoxVAO()->Unbind();
}

Gizmos::~Gizmos()
{
	for (auto elem : this->raysInTheScene)
	{
		delete elem.second;
	}
	this->raysInTheScene.clear();
}