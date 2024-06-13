#pragma once
#include <GL/glew.h>

class VBO
{
public:
	unsigned int ID;

	VBO();
	template<typename T>
	void Data(T* vertic, GLsizeiptr size);
	void Bind();
	void UnBind();
	void Delete();
};

template<typename T>
inline void VBO::Data(T* vertic, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertic, GL_STATIC_DRAW);
}
