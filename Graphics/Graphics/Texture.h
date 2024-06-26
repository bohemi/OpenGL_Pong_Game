#pragma once
#include <GL/glew.h>
#include "MyFolder/IMGLoader/stb_image.h"
#include "shader.h"

class Texture
{
public:

	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, unsigned int unit);

	void ActivateTex(GLenum textureNumber);

	// Binds a texture
	void Bind();

	// Unbinds a texture
	void Unbind();

	// Deletes a texture
	void Delete();
};

