#pragma once
#include <GL/glew.h>

class EBO
{
public:
	unsigned int ID;

	EBO(unsigned int* indices, GLsizeiptr size);

	void Bind();
	void UnBind();
	void Delete();
};

