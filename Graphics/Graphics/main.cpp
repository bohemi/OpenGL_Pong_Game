#include "Manager.h"
#include "shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include <vector>

// settings
GLFWwindow* window = nullptr;
const int width = 800.0f, height = 600.0f;

// bat speed and key events
float testSpeed = 2;

bool isLeftBatUp = false;
bool isLeftBatDown = false;
bool isRighBatUp = false;
bool isRighBatDown = false;

// ball speed and key events
float ballSpeedVertical = 1.0f;
float ballSpeedHorizontal = 0.5f;
bool activateBallUpSpeed = false;
// box left settings
glm::vec2 batPositionLeft(0.0f);
glm::vec2 batScaleLeft(0.0f);
// box right settings
glm::vec2 batPositionRight(0.0f);
glm::vec2 batScaleRight(0.0f);

// Ball settings
glm::vec2 ballPosition(0.0f);
glm::vec2 ballScale(0.0f);

bool isWithinBounds(const glm::vec2& boxPos, float boxSize) // will check inside BatMovement function
{
	const float halfSize = boxSize / 2.0f;
	float topEdge = boxPos.y + halfSize;
	float bottomEdge = boxPos.y - halfSize;

	return (topEdge <= 100.0f && bottomEdge >= 0.0f);
}

void BatMovement(float& speed, glm::vec2& batPosLeft, glm::vec2& batPosRight, float boxSize)
{
	if (isLeftBatUp)
	{
		glm::vec2 boxPos = batPosLeft;
		boxPos.y += speed;

		if (isWithinBounds(boxPos, boxSize))
			batPosLeft = boxPos;

		isLeftBatUp = false;
	}
	if (isLeftBatDown)
	{
		glm::vec2 boxPos = batPosLeft;
		boxPos.y -= speed;

		if (isWithinBounds(boxPos, boxSize))
			batPosLeft = boxPos;

		isLeftBatDown = false;
	}

	if (isRighBatUp)
	{
		glm::vec2 boxPos = batPosRight;
		boxPos.y += speed;

		if (isWithinBounds(boxPos, boxSize))
			batPosRight = boxPos;

		isRighBatUp = false;
	}
	if (isRighBatDown)
	{
		glm::vec2 boxPos = batPosRight;
		boxPos.y -= speed;

		if (isWithinBounds(boxPos, boxSize))
			batPosRight = boxPos;

		isRighBatDown = false;
	}
}

void BallMovement()
{
	ballPosition.x += ballSpeedHorizontal;
	ballPosition.y += ballSpeedVertical;

	if (ballPosition.x + ballScale.x >= batPositionRight.x && ballPosition.y + ballScale.y / 2.0f <= 
		batPositionRight.y + batScaleRight.y / 2.0f && ballPosition.y - ballScale.y / 2.0f >=
		batPositionRight.y - batScaleRight.y / 2.0f)
	{
		ballSpeedHorizontal = -1.0f;
		std::cout << "going Left\n";
	}
	else if ((ballPosition.x - ballScale.x <= batPositionLeft.x && ballPosition.y + ballScale.y / 2.0f <=
		batPositionLeft.y + batScaleLeft.y / 2.0f && ballPosition.y - ballScale.y / 2.0f >=
		batPositionLeft.y - batScaleLeft.y / 2.0f))
	{
		ballSpeedHorizontal = 1.0f;
		std::cout << "going Right\n";
	}

	if (ballPosition.y + ballScale.y / 2.0f >= 100.0f)
	{
		ballSpeedVertical = -0.7f;
	}
	else if (ballPosition.y - ballScale.y / 2.0f <= 0.0f)
	{
		ballSpeedVertical = 0.7f;
	}
}

void RestartGame()
{
	if (ballPosition.x > 101.0f || ballPosition.x < -1.0f)
	{
		ballPosition = glm::vec2(50.0f, 50.0f);
		ballSpeedVertical = 0.0f;
		ballSpeedHorizontal = 0.7f;
		activateBallUpSpeed = true;
	}
	if (ballPosition.x + ballScale.x >= batPositionRight.x && activateBallUpSpeed)
	{
		ballSpeedVertical = 0.7f;
		activateBallUpSpeed = false;
	}
}

void HandleInputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		isLeftBatUp = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		isLeftBatDown = true;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		isRighBatUp = true;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		isRighBatDown = true;
}

void ConstructCircle(std::vector<glm::vec3>&vertices, int vCount)
{
	float angle = 360.0f / vCount;

	int triangleCount = vCount - 2;

	std::vector<glm::vec3> temp;
	// positions
	for (int i = 0; i < vCount; i++)
	{
		float currentAngle = angle * i;
		float x = 1 * cos(glm::radians(currentAngle));
		float y = 1 * sin(glm::radians(currentAngle));
		float z = 0.0f;

		temp.push_back(glm::vec3(x, y, z));
	}

	for (int i = 0; i < triangleCount; i++)
	{
		vertices.push_back(temp[0]);
		vertices.push_back(temp[i + 1]);
		vertices.push_back(temp[i + 2]);
	}
}

void DrawCircle(glm::mat4& model, Shader& shader, float uniformScale, float xTrans, unsigned int count)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(xTrans, 0.0f + uniformScale, 0.0f));
	model = glm::scale(model, glm::vec3(uniformScale, uniformScale, 0.0f));
	shader.setMat4("model", model);
	// Draw
	glDrawArrays(GL_TRIANGLES, 0, count); // set and send the matrix before draw
}

void DrawBat(glm::mat4& model, Shader& shader, glm::vec2 &position, glm::vec2 &scale)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(scale, 0.0f));
	shader.setMat4("model", model);

	// Draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // set and send the matrix before draw
}

int main()
{
	Manager manager(window, width, height, "Pong", false);

	Shader shader("MyFolder/ShaderFiles/Pong.vs", "MyFolder/ShaderFiles/Pong.fs");

	// circle construction
	/*std::vector<glm::vec3>circleVertices;
	ConstructCircle(circleVertices, 128);
	VAO VAO2;
	VAO2.Bind();
	VBO VBO2;
	VBO2.Data(&circleVertices[0], sizeof(glm::vec3) * circleVertices.size());
	VAO2.LinkAttributes(VBO2, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);*/

	// Bats pos
	float batPosition[12] =
	{
		-0.5f, -0.5f, 0.0f,  // bottom left
		 0.5f, -0.5f, 0.0f,  // bottom right
		 0.5f,  0.5f, 0.0f,  // top right
		-0.5f,  0.5f, 0.0f   // top left
	};
	unsigned int batIndices[6] =
	{
		0, 1, 2,  // first Triangle
		2, 3, 0   // second Triangle
	};

	VAO VAO1; // default constructor will generate vertexArray
	VAO1.Bind();
	VBO VBO1;
	VBO1.Data(batPosition, sizeof(batPosition));
	EBO EBO1(batIndices, sizeof(batIndices)); // will generate, bind and send buffer data
	VAO1.LinkAttributes(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	// UnBinding
	VAO1.UnBind();
	VBO1.UnBind();
	EBO1.UnBind();

	// Transformations
	glm::mat4 projection(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 model(1.0f); // Aways set to identity everytime before using

	// Dont need to be in loop every time
	projection = glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, 0.1f, 100.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	// Activate Shaders
	shader.use();

	// Dont need to be in loop every time
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	// setting bats`s/ball`s scale and transforms
	batPositionLeft = glm::normalize(batPositionLeft);
	batPositionLeft = glm::vec2(1.5f, 55.0f);
	batScaleLeft = glm::vec2(1.5f, 15.0f);

	batPositionRight = glm::normalize(batPositionRight);
	batPositionRight = glm::vec2(98.5f, 55.0f);
	batScaleRight = glm::vec2(1.5f, 15.0f);

	ballPosition = glm::normalize(ballPosition);
	ballPosition = glm::vec2(50.0f, 50.0f);
	ballScale = glm::vec2(1.5f, 1.9f);

	while (!glfwWindowShouldClose(window))
	{
		// Input
		HandleInputs(window);

		// Rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT); // clears buffer with depth/color etc.

		// Bats
		VAO1.Bind();
		// Move and check Box colliders
		DrawBat(model, shader, batPositionLeft, batScaleLeft); // Bat Left

		DrawBat(model, shader, batPositionRight, batScaleRight); // right bat

		DrawBat(model, shader, ballPosition, ballScale); // ball

		BatMovement(testSpeed, batPositionLeft, batPositionRight, batScaleLeft.y);

		BallMovement();

		RestartGame();

		// circle
		//VAO2.Bind();
		//DrawCircle(model, shader, 3.0f, -testSpeed, circleVertices.size());

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// cleanup
	VAO1.Delete();
	VBO1.Delete();
	//VAO2.Delete();
	//VBO2.Delete();
	glfwTerminate();
	return 0;
}