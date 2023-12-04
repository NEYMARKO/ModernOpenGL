#include "Mesh.h"
#include "VAO.h"
#include "Lighting.h"
#include "VBO.h"

Lighting::Lighting(glm::vec3 position, Mesh& mesh) : meshContainer(mesh)
{
	this->position = position;
	this->lightVAO = mesh._VAO;
	//InitializeShape();

}

void Lighting::InitializeShape()
{
	/*lightVAO.Bind();
	unsigned int VBO = this->meshContainer.getVBO();
	unsigned int EBO = this->meshContainer.getEBO();
	std::cout << "EBO: " << EBO << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshContainer.indices.size() * sizeof(unsigned int), &this->meshContainer.indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	lightVAO.Unbind();*/
	std::cout << "Nothing should be happening here" << std::endl;
}

void Lighting::BindVAO()
{
	lightVAO.Bind();
}

void Lighting::UnbindVAO()
{
	lightVAO.Unbind();
}

void Lighting::Draw()
{
	BindVAO();
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->meshContainer.getEBO());
	glDrawElements(GL_TRIANGLES, this->meshContainer.indices.size(), GL_UNSIGNED_INT, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	UnbindVAO();
}
glm::mat4 Lighting::ModelMatrix()
{
	glm::mat4 model;
	model = glm::translate(model, this->position);
	return model;
}