#pragma once
#include "GL/glew.h"
#include "foundation/math/vec4.h"
#include <memory>



namespace Render 
{
class ShaderObject;
}

namespace Render
{
class LightNode;
}

namespace Display
{
class Window;
}

namespace Input
{

class InputManager
{

public:
	InputManager();
	InputManager(Display::Window* window, Render::LightNode* lNode);
	~InputManager();

	void InitKeyPress();
	void InitMouse();

	void Update();

	GLfloat GetRotX();
	GLfloat GetRotY();

	GLfloat GetOldX();
	GLfloat GetOldY();

	Math::vec4 GetMovement();
	void CameraMovement();

private:
	Display::Window* window;

	GLfloat rotY = 0;
	GLfloat rotX = 0;
	GLfloat oldY = 0;
	GLfloat oldX = 0;
	GLfloat movSpeed = 1.0f;
	GLfloat movMultiplier = 3.5f;
	GLfloat scrollY = 0;
	GLfloat lightScroll = 0;
	GLfloat indexScroll = 0;

	Math::vec4 vec;
	float mouseX, mouseY;

	Math::vec4 movement;

	Render::LightNode* lNode;

	GLboolean wFlag, sFlag, dFlag, aFlag, shiftFlag, mLeftCheck, mRightCheck;
};

}
