#include "Manager.h"
#include "shader.h"
#include "Objects.h"
#include "AudioManager.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include <vector>
#include<fmod.hpp>
#include<fmod.h>
#include<fmod_errors.h>

// settings
GLFWwindow* window = nullptr;
const int width = 800.0f, height = 600.0f;

// bat and ball speed
float batSpeed = 1.0f;
float ballSpeedVertical = 1.0f;
float ballSpeedHorizontal = 0.5f;
bool activateBallUpSpeed = false;

// box left settings
glm::vec3 batPositionLeft(0.0f); glm::vec2 batScaleLeft(0.0f);
// box right settings
glm::vec3 batPositionRight(0.0f); glm::vec2 batScaleRight(0.0f);

// Ball settings
glm::vec3 ballPosition(0.0f); glm::vec2 ballScale(0.0f);

// Audio settings
bool batSound = false;
bool wallSound = false;

void BallMovement()
{
	ballPosition.x += ballSpeedHorizontal;
	ballPosition.y += ballSpeedVertical;

	// bat right
	// with extra condtions to prevent ball glitching through the bats when its below/above the bat which
	// was making the ball glitch inside the bats. so when the ball hits any corner of the bat it will
	// now simply go vertically opposite side it came from.
	if (ballPosition.x + ballScale.x / 2.0f >= batPositionRight.x - batScaleRight.x / 2.0f)
	{
		if (ballPosition.y + ballScale.y / 2.0f <= batPositionRight.y + batScaleRight.y / 2.0f &&
			ballPosition.y - ballScale.y / 2.0f >= batPositionRight.y - batScaleRight.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = -1.0f;
			std::cout << "going Left\n";

		}
		else if (ballPosition.y - ballScale.y / 2.0f <= batPositionRight.y + batScaleRight.y / 2.0f &&
			ballPosition.y + ballScale.y / 2.0f >= batPositionRight.y + batScaleRight.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = -1.0f;
			ballSpeedVertical = 1.0f;
			std::cout << "going Left UP\n";
		}
		else if (ballPosition.y + ballScale.y / 2.0f >= batPositionRight.y - batScaleRight.y / 2.0f &&
			ballPosition.y - ballScale.y / 2.0f <= batPositionRight.y - batScaleRight.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = -1.0f;
			ballSpeedVertical = -1.0f;
			std::cout << "going Left Down\n";
		}
	}

	// bat Left
	if (ballPosition.x - ballScale.x / 2.0f <= batPositionLeft.x + batScaleLeft.x / 2.0f)
	{
		if (ballPosition.y + ballScale.y / 2.0f <= batPositionLeft.y + batScaleLeft.y / 2.0f &&
			ballPosition.y - ballScale.y / 2.0f >= batPositionLeft.y - batScaleLeft.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = 1.0f;
			std::cout << "going Right\n";

		}
		else if (ballPosition.y - ballScale.y / 2.0f <= batPositionLeft.y + batScaleLeft.y / 2.0f &&
			ballPosition.y + ballScale.y / 2.0f >= batPositionLeft.y + batScaleLeft.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = 1.0f;
			ballSpeedVertical = 1.0f;
			std::cout << "going Right UP\n";
		}
		else if (ballPosition.y + ballScale.y / 2.0f >= batPositionLeft.y - batScaleLeft.y / 2.0f &&
			ballPosition.y - ballScale.y / 2.0f <= batPositionLeft.y - batScaleLeft.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = 1.0f;
			ballSpeedVertical = -1.0f;
			std::cout << "going Right Down\n";
		}
	}
	// vertical walls
	// Issue => its being checked more than once making the sound play twice(solved)
	// changing ball at Y in first iteration will prevent checking more than once
	if (ballPosition.y + ballScale.y / 2.0f >= 100.0f)
	{
		ballPosition.y -= 0.5f;
		ballSpeedVertical = -0.7f;
		wallSound = true;
	}
	else if (ballPosition.y - ballScale.y / 2.0f <= 0.0f)
	{
		ballPosition.y += 0.5f;
		ballSpeedVertical = 0.7f;
		wallSound = true;
	}
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

int main()
{
	// Init OpenGL
	Manager manager(window, width, height, "Pong", false);
	Objects objLeft;
	Objects objRight;
	Objects objBall;

	Shader shader("MyFolder/ShaderFiles/Pong.vs", "MyFolder/ShaderFiles/Pong.fs");
	
	// Init Audio
	FMOD_RESULT result;
	FMOD::System* system = nullptr;
	FMOD::Sound* sound = nullptr;
	
	AudioManager audio(result, system, sound);

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
	batPositionLeft = glm::vec3(1.5f, 50.0f, 0.0f);
	batScaleLeft = glm::vec2(1.5f, 15.0f);

	batPositionRight = glm::normalize(batPositionRight);
	batPositionRight = glm::vec3(98.5f, 50.0f, 0.0f);
	batScaleRight = glm::vec2(1.5f, 15.0f);

	ballPosition = glm::normalize(ballPosition);
	ballPosition = glm::vec3(50.0f, 50.0f, 0.0f);
	ballScale = glm::vec2(1.5f, 1.9f);

	while (!glfwWindowShouldClose(window))
	{
		// Input
		manager.KeyEvents(window);

		// Audio
		manager.HandleAudio(audio, system, sound, batSound, wallSound);

		// Rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT); // clears buffer with depth/color etc.

		// Bats
		VAO1.Bind();

		objLeft.MoveObjects(manager, batPositionLeft, batPositionRight, batScaleLeft.y, batSpeed);
		objLeft.DrawObjects(model, shader, batPositionLeft, batScaleLeft);
		objRight.DrawObjects(model, shader, batPositionRight, batScaleRight);
		objBall.DrawObjects(model, shader, ballPosition, ballScale);

		BallMovement();

		manager.RestartGame(ballPosition, ballScale, batPositionRight, batScaleRight, 
			ballSpeedVertical, ballSpeedHorizontal, activateBallUpSpeed);

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
	sound->release();
	system->release();
	//VAO2.Delete();
	//VBO2.Delete();
	glfwTerminate();
	return 0;
}