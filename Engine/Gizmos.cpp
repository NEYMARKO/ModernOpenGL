#include "Camera.h"
#include "Shader.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "PrintClass.h"
#include "Gizmos.h"
Gizmos::Gizmos(Camera* camera, Shader* meshShader, Shader* boundingBoxShader, Shader* pointShader)
{
	this->camera = camera;
	this->meshShader = meshShader;
	this->boundingBoxShader = boundingBoxShader;
	this->pointShader = pointShader;
}

void Gizmos::AddPoint(const glm::vec3& position)
{
	this->pointsPositions.push_back(position);
	//SetupPointsBuffer();
}
void Gizmos::SetupPointsBuffer()
{
	this->pointsVAOBuffer.Bind();
	this->pointsVBOBuffer = VBO(this->pointsPositions);

	this->pointsVAOBuffer.LinkVBO(this->pointsVBOBuffer, 0, 3, sizeof(glm::vec3), 0);

	this->pointsVAOBuffer.Unbind();
	this->pointsVBOBuffer.Unbind();
}

void Gizmos::UpdatePoints(const std::vector<glm::vec3>& positions)
{
	this->pointsPositions = positions;

	/*for (glm::vec3 point : positions)
	{
		PrintClass::PrintVec3(point);
	}*/
	this->pointsVBOBuffer.Delete();
	SetupPointsBuffer();
}
void Gizmos::RenderPoints(float radius)
{
	this->pointShader->Activate();

	glm::mat4 model = glm::mat4(1.0f);
	this->pointShader->SetMat4("model", model);

	this->camera->generateViewProjectionMatrices(*(this->boundingBoxShader));

	glDisable(GL_DEPTH_TEST);
	this->pointsVAOBuffer.Bind();

	glDrawArrays(GL_POINTS, 0, this->pointsPositions.size());

	this->pointsVAOBuffer.Unbind();
	glEnable(GL_DEPTH_TEST);
}
void Gizmos::AddRay(std::string name, const glm::vec3& startPos, const glm::vec3& direction, int length)
{
	Ray* newRay = new Ray(startPos, direction, length);
	this->raysInTheScene.insert({name, newRay});
}

//position is getting fetched from world space => no need for converting
void Gizmos::RenderLine(Ray* ray, glm::vec3& color)
{

	this->boundingBoxShader->Activate();

	glLineWidth(15.0f);
	//Coordinates are already converted to world space in Raycast function
	glm::mat4 model = glm::mat4(1.0f);

	this->boundingBoxShader->SetMat4("model", model);
	this->camera->generateViewProjectionMatrices(*(this->boundingBoxShader));

	this->boundingBoxShader->SetVec3("lineColor", color);

	ray->GetRayVAO()->Bind();
	glDrawElements(GL_LINES, ray->GetRayIndices()->size(), GL_UNSIGNED_INT, 0);
	ray->GetRayVAO()->Bind();
}

void Gizmos::RenderAllLines(glm::vec3& color)
{
	/*std::cout << "NUMBER OF LINES: " << this->raysInTheScene.size() << std::endl;*/
	for (auto ray : this->raysInTheScene)
	{
		/*std::cout << "NAME: " << ray.first << std::endl;*/
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

	//glm::mat4 model = boundingBox->GetParentMesh()->GetFinalMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	this->boundingBoxShader->SetMat4("model", model);
	this->boundingBoxShader->SetVec3("cameraPos", this->camera->GetCameraPosition());
	this->boundingBoxShader->SetVec3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));

	this->camera->generateViewProjectionMatrices(*(this->boundingBoxShader));

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