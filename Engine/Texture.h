#pragma once
#include <iostream>
#include <stb/stb_image.h>
#include <glad/glad.h>

class Texture
{
	public:
		GLuint ID;
		Texture(const char* image);

		void Bind();
		void Unbind();
		void Delete();
};