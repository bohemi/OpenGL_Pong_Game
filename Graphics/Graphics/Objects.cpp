#include "Objects.h"

void Objects::DrawObjects(glm::mat4& model, Shader& shader, glm::vec3& position, glm::vec2& scale)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position));
	model = glm::scale(model, glm::vec3(scale, 0.0f));
	shader.setMat4("model", model);
	// Draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // set and send the matrix before draw
}

bool Objects::ObjectBounds(glm::vec3 objPos, float objSize)
{
	const float halfSize = objSize / 2.0f;
	float topEdge = objPos.y + halfSize;
	float bottomEdge = objPos.y - halfSize;

	return (topEdge <= 100.0f && bottomEdge >= 0.0f);
}

void Objects::MoveObjects(Manager& manager, glm::vec3& batPosLeft, glm::vec3& batPosRight, float size, float speed)
{
	if (manager.leftBatUp)
	{
		glm::vec3 boxPos = batPosLeft;
		boxPos.y += speed;
		if (ObjectBounds(boxPos, size))
			batPosLeft = boxPos;
	}
	if (manager.leftBatDown)
	{
		glm::vec3 boxPos = batPosLeft;
		boxPos.y -= speed;

		if (ObjectBounds(boxPos, size))
			batPosLeft = boxPos;
	}

	if (manager.rightBatUp)
	{
		glm::vec3 boxPos = batPosRight;
		boxPos.y += speed;

		if (ObjectBounds(boxPos, size))
			batPosRight = boxPos;
	}
	if (manager.rightBatDown)
	{
		glm::vec3 boxPos = batPosRight;
		boxPos.y -= speed;

		if (ObjectBounds(boxPos, size))
			batPosRight = boxPos;
	}
}