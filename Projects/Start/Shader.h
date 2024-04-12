#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>
#include "PrintClass.h"

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	Shader();
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);
	// Checks for shaders linking errors
	void checkCompileErrors(GLuint shader, std::string shaderType);
	// Checks for shader program linking errors
	void checkProgramLinkErrors(GLuint shaderProgram);
	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	void SetFloat(const std::string& name, const float value);

	void SetVec2(const std::string& name, const glm::vec2& value);

	void SetVec3(const std::string& name, const glm::vec3& value);

	void SetVec4(const std::string& name, const glm::vec4& value);

	void SetMat3(const std::string& name, const glm::mat3& value);

	void SetMat4(const std::string& name, const glm::mat4& value);
};
