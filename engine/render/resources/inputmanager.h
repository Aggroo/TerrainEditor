#pragma once
#include "GL/glew.h"
#include "foundation/math/vector4D.h"
#include <memory>

namespace CGMath {
	class LightNode;
	class ShaderObject;
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
	InputManager(Display::Window* window, CGMath::ShaderObject* shaders, CGMath::LightNode* lNode);
	~InputManager();

	void InitKeyPress();
	void InitMouse();

	void Update();

	GLfloat GetRotX();
	GLfloat GetRotY();

	GLfloat GetOldX();
	GLfloat GetOldY();

	CGMath::vector4D GetMovement();

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

	CGMath::vector4D movement;

	CGMath::ShaderObject* shaders;
	CGMath::LightNode* lNode;

	GLboolean wFlag, sFlag, dFlag, aFlag, shiftFlag, mLeftCheck, mRightCheck;
};

}
