#pragma once
#include "Shader.h"
#include "Manager.h"

class Objects
{
public:

	void DrawObjects(glm::mat4& model, Shader& shader, glm::vec3& position, glm::vec2& scale);
	bool ObjectBounds(glm::vec3 objPos,float objSize);
	void MoveObjects(Manager& manager, glm::vec3& batPosLeft, glm::vec3& batPosRight, float size, float speed);
};