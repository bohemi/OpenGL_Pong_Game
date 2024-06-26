#include "Manager.h"
#include "shader.h"
#include "Objects.h"
#include "AudioManager.h"
#include "Texture.h"
#include "VAO.h"
#include "EBO.h"
#include <vector>
#include<fmod.hpp>
#include<fmod.h>
#include<fmod_errors.h>

// settings
GLFWwindow* window = nullptr;
const int width = 800.0f, height = 600.0f;

// bat and ball setting
float batSpeed = 1.0f;
float ballSpeedVertical = 1.0f;
float ballSpeedHorizontal = 0.5f;
bool activateBallUpSpeed = false;

// box left Transformations
glm::vec3 batPositionLeft(0.0f); glm::vec2 batScaleLeft(0.0f);
// box right Transformations
glm::vec3 batPositionRight(0.0f); glm::vec2 batScaleRight(0.0f);
// circle Transformations
glm::vec3 circlePosition(0.0f); glm::vec2 circleScale(0.0f);
// circle vertices
std::vector<glm::vec3>circleVertices;

// Audio settings
bool batSound = false;
bool wallSound = false;

void CircleMovement()
{
	circlePosition.x += ballSpeedHorizontal;
	circlePosition.y += ballSpeedVertical;

	// bat right
	// with extra condtions to prevent ball glitching through the bats when its below/above the bat which
	// was making the ball glitch inside the bats. so when the ball hits any corner of the bat it will
	// now simply go vertically opposite side it came from.
	if (circlePosition.x + circleScale.x / 2.0f >= batPositionRight.x - batScaleRight.x / 2.0f)
	{
		if (circlePosition.y + circleScale.y / 2.0f <= batPositionRight.y + batScaleRight.y / 2.0f &&
			circlePosition.y - circleScale.y / 2.0f >= batPositionRight.y - batScaleRight.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = -1.0f;
			std::cout << "going Left\n";

		}
		else if (circlePosition.y - circleScale.y / 2.0f <= batPositionRight.y + batScaleRight.y / 2.0f &&
			circlePosition.y + circleScale.y / 2.0f >= batPositionRight.y + batScaleRight.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = -1.0f;
			ballSpeedVertical = 1.0f;
			std::cout << "going Left UP\n";
		}
		else if (circlePosition.y + circleScale.y / 2.0f >= batPositionRight.y - batScaleRight.y / 2.0f &&
			circlePosition.y - circleScale.y / 2.0f <= batPositionRight.y - batScaleRight.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = -1.0f;
			ballSpeedVertical = -1.0f;
			std::cout << "going Left Down\n";
		}
	}
	// bat Left
	if (circlePosition.x - circleScale.x / 2.0f <= batPositionLeft.x + batScaleLeft.x / 2.0f)
	{
		if (circlePosition.y + circleScale.y / 2.0f <= batPositionLeft.y + batScaleLeft.y / 2.0f &&
			circlePosition.y - circleScale.y / 2.0f >= batPositionLeft.y - batScaleLeft.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = 1.0f;
			std::cout << "going Right\n";

		}
		else if (circlePosition.y - circleScale.y / 2.0f <= batPositionLeft.y + batScaleLeft.y / 2.0f &&
			circlePosition.y + circleScale.y / 2.0f >= batPositionLeft.y + batScaleLeft.y / 2.0f)
		{
			batSound = true;
			ballSpeedHorizontal = 1.0f;
			ballSpeedVertical = 1.0f;
			std::cout << "going Right UP\n";
		}
		else if (circlePosition.y + circleScale.y / 2.0f >= batPositionLeft.y - batScaleLeft.y / 2.0f &&
			circlePosition.y - circleScale.y / 2.0f <= batPositionLeft.y - batScaleLeft.y / 2.0f)
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
	if (circlePosition.y + circleScale.y / 2.0f >= 100.0f)
	{
		circlePosition.y -= 0.5f;
		ballSpeedVertical = -0.7f;
		wallSound = true;
	}
	else if (circlePosition.y - circleScale.y / 2.0f <= 0.0f)
	{
		circlePosition.y += 0.5f;
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

void DrawCircle(glm::mat4& model, Shader& shader, glm::vec3 position, glm::vec2 scale, unsigned int count)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position));
	model = glm::scale(model, glm::vec3(scale, 0.0f));
	shader.setMat4("model", model);
	// Draw
	glDrawArrays(GL_TRIANGLES, 0, count); // set and send the matrix before draw
}

int main()
{
	// Init OpenGL
	Manager manager(window, width, height, "Pong", false);
	// Init objects
	Objects objLeft;
	Objects objRight;
	Objects objBall;
	Shader shader("MyFolder/ShaderFiles/Pong.vs", "MyFolder/ShaderFiles/Pong.fs");

	// Init texture
	Texture batLeftTexture("MyFolder/textures/leftBatIMG.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	Texture batRightTexture("MyFolder/textures/rightBatIMG.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
	
	// Init Audio
	FMOD_RESULT result;
	FMOD::System* system = nullptr;
	FMOD::Sound* sound = nullptr;
	AudioManager audio(result, system, sound);
	
	// circle construction
	
	ConstructCircle(circleVertices, 128);

	VAO VAOBall;
	VAOBall.Bind();
	VBO VBOBall;
	VBOBall.Data(&circleVertices[0], sizeof(glm::vec3) * circleVertices.size());
	VAOBall.LinkAttributes(VBOBall, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	// Bats pos
	float batPosition[20] =
	{
		// positions		// tex coords
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // bottom left
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  // top right
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left
	};

	unsigned int batIndices[6] =
	{
		0, 1, 2,  // first Triangle
		2, 3, 0   // second Triangle
	};

	VAO VAOBat; // default constructor will generate vertexArray
	VAOBat.Bind();
	VBO VBOBat;
	VBOBat.Data(batPosition, sizeof(batPosition));
	EBO EBOBat(batIndices, sizeof(batIndices)); // will generate, bind and send buffer data
	// position attributes
	VAOBat.LinkAttributes(VBOBat, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	// texture attributes
	VAOBat.LinkAttributes(VBOBat, 1, 3, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// set texture units
	batRightTexture.texUnit(shader, "tex0", 0);
	batLeftTexture.texUnit(shader, "tex1", 1);

	// UnBinding
	VAOBat.UnBind();
	VBOBat.UnBind();
	EBOBat.UnBind();

	VAOBall.UnBind();
	VBOBall.UnBind();

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

	// circle
	circlePosition = glm::normalize(circlePosition);
	circlePosition = glm::vec3(50.0f, 50.0f, 0.0f);
	circleScale = glm::vec2(1.0f, 1.0f);

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
		VAOBat.Bind();

		batLeftTexture.texUnit(shader, "drawIndex", 0);
		batLeftTexture.ActivateTex(GL_TEXTURE0);
		batLeftTexture.Bind();

		objLeft.DrawObjects(model, shader, batPositionLeft, batScaleLeft);
		
		batRightTexture.texUnit(shader, "drawIndex", 1);
		batRightTexture.ActivateTex(GL_TEXTURE1);
		batRightTexture.Bind();

		objRight.DrawObjects(model, shader, batPositionRight, batScaleRight);

		// circle
		VAOBall.Bind();
		shader.setInt("drawIndex", 2);
		DrawCircle(model, shader, circlePosition, circleScale, circleVertices.size());

		// movements
		objLeft.MoveObjects(manager, batPositionLeft, batPositionRight, batScaleLeft.y, batSpeed);
		CircleMovement();

		
		manager.RestartGame(circlePosition, circleScale, batPositionRight, batScaleRight,
			ballSpeedVertical, ballSpeedHorizontal, activateBallUpSpeed);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// cleanup
	VAOBat.Delete();
	VBOBat.Delete();
	VAOBall.Delete();
	VBOBall.Delete();
	sound->release();
	system->release();
	glfwTerminate();
	return 0;
}