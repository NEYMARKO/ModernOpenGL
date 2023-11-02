#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
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
};
