#include "Mesh.h"
#include "BoundingBox.h"
BoundingBox::BoundingBox(glm::vec3 minExtremes, glm::vec3 maxExtremes, Mesh* parentMesh) : 
	mLocalMinExtremes{ minExtremes }, mLocalMaxExtremes{ maxExtremes }, mParentMesh{ parentMesh } 
{
	Initialize();
	SetupBuffers();
	VerticesToWorld();
}
BoundingBox::~BoundingBox()
{
	mBoxVAO.Delete();
	mBoxVBO.Delete();
	mBoxEBO.Delete();
	std::cout << "DELETED BOUNDING BOX" << std::endl;
}
void BoundingBox::Initialize()
{
	float xmin = this->mLocalMinExtremes[0];
	float ymin = this->mLocalMinExtremes[1];
	float zmin = this->mLocalMinExtremes[2];
	float xmax = this->mLocalMaxExtremes[0];
	float ymax = this->mLocalMaxExtremes[1];
	float zmax = this->mLocalMaxExtremes[2];
	this->mVertices = {
		glm::vec3(xmin, ymin, zmin), // 0
		glm::vec3(xmax, ymin, zmin), // 1
		glm::vec3(xmax, ymax, zmin), // 2
		glm::vec3(xmin, ymax, zmin), // 3
		glm::vec3(xmin, ymin, zmax), // 4
		glm::vec3(xmax, ymin, zmax), // 5
		glm::vec3(xmax, ymax, zmax), // 6
		glm::vec3(xmin, ymax, zmax)  // 7
	};

	this->mIndices = {
		// Bottom face
		0, 1, 1, 2, 2, 3, 3, 0,
		// Top face
		4, 5, 5, 6, 6, 7, 7, 4,
		// Side edges
		0, 4, 1, 5, 2, 6, 3, 7
	};

}

void BoundingBox::SetupBuffers()
{
	this->mBoxVAO.Bind();
	this->mBoxVBO = VBO(this->mVertices);
	this->mBoxEBO = EBO(this->mIndices);

	this->mBoxVAO.LinkVBO(mBoxVBO, 0, 3, sizeof(glm::vec3), 0);

	this->mBoxVAO.Unbind();
	this->mBoxVBO.Unbind();
	this->mBoxEBO.Unbind();

}


void BoundingBox::VerticesToWorld()
{
	glm::mat4 model = mParentMesh->GetFinalMatrix();
	this->mMinExtremes = glm::vec3(model * glm::vec4(this->mLocalMinExtremes, 1.0));
	this->mMaxExtremes = glm::vec3(model * glm::vec4(this->mLocalMaxExtremes, 1.0));
}

bool BoundingBox::Intersects(Camera& camera, float step)
{
	Ray* ray = camera.mRay;

	glm::vec3 rayPoint = ray->GetRayStart() + ray->GetRayDirection() * step;
	for (int i = 0; i < 3; i++)
	{
		if (rayPoint[i] > this->mMaxExtremes[i] || rayPoint[i] < this->mMinExtremes[i]) return false;
	}
	
	std::cout << "INTERSECT FOR object: " << mParentMesh->GetID() << std::endl;
	return true;
}

void BoundingBox::Draw(Shader& shaderProgram, Camera& camera)
{
	shaderProgram.Activate();
	
	glm::mat4 model = mParentMesh->GetFinalMatrix();
	shaderProgram.SetMat4("model", model);
	shaderProgram.SetVec3("cameraPos", camera.GetCameraPosition());
	shaderProgram.SetVec3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));

	camera.ViewProjectionMatrix(shaderProgram);

	mBoxVAO.Bind();

	glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, 0);

	mBoxVAO.Unbind();
}

glm::vec3 BoundingBox::GetMinExtremes()
{
	return this->mMinExtremes;
}

glm::vec3 BoundingBox::GetMaxExtremes()
{
	return this->mMaxExtremes;
}

VAO* BoundingBox::GetBoundingBoxVAO()
{
	return &this->mBoxVAO;
}
Mesh* BoundingBox::GetParentMesh()
{
	return mParentMesh;
}
std::vector<unsigned int>* BoundingBox::GetIndices()
{
	return &this->mIndices;
}