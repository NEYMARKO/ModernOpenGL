#include "Grid.h"
#define STEP 0.5
Grid::Grid(int size)
{
	this->size = size;
	SetupGrid();
}

void Grid::SetupGrid()
{
	/*std::cout << "size: " << this->size << std::endl;
	std::cout << "Step: " << STEP << std::endl;*/
	for (float i = 0; i <= this->size; i += STEP)
	{
		for (float j = 0; j <= this->size; j += STEP)
		{ 
			//std::cout << "Grid (i, j): " << "(" << i << ", " << j << ")" << std::endl;
			this->vertices.push_back(glm::vec3(j, 0, i));
		}
	}

	int length = this->size * (1 / STEP);
	for (float i = 0; i < length; i++)
	{
		for (float j = 0; j < length; j++)
		{
			//horizontal indices
			this->indices.push_back(i * length + j + i);
			this->indices.push_back(i * length + j + i + 1);
			//vertical indices
			this->indices.push_back(i * length + j + i);
			this->indices.push_back((i + 1) * length + j + i + 1);
		}
	}

	this->modelMatrix = glm::mat4(1.0f);
	int stepOffset = 1 / STEP;
	this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(-length / (2 * stepOffset), 0.0f, -length / (2 * stepOffset)));

	this->gridVAO.Bind();
	this->gridVBO = VBO(this->vertices);
	this->gridEBO = EBO(this->indices);

	this->gridVAO.LinkVBO(gridVBO, 0, 3, sizeof(glm::vec3), 0);

	this->gridVAO.Unbind();
	this->gridVBO.Unbind();
	this->gridEBO.Unbind();
}
void Grid::Draw(Shader& shaderProgram, Camera& camera)
{
	//std::cout << "Drawing grid" << std::endl;
	shaderProgram.Activate();

	glLineWidth(2.0f);

	shaderProgram.SetMat4("model", this->modelMatrix);
	shaderProgram.SetVec3("cameraPos", camera.GetCameraPosition());
	shaderProgram.SetVec3("lineColor", glm::vec3(0.329412f, 0.329412f, 0.329412f));

	camera.ViewProjectionMatrix(shaderProgram);

	gridVAO.Bind();

	glDrawElements(GL_LINES, this->indices.size(), GL_UNSIGNED_INT, 0);

	gridVAO.Unbind();

	glLineWidth(1.0f);

}

Grid::~Grid()
{
	std::cout << "Grid deleted" << std::endl;
}