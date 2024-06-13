#pragma once
#include "VBO.h"

class VAO
{
public:
	unsigned int ID;
	
	VAO();
	void LinkAttributes(VBO& VBO, unsigned int layout, unsigned int num_of_components, GLenum type, size_t stride, void* offset);

	void Bind();
	void UnBind();
	void Delete();
};

