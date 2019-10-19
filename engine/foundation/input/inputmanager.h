#pragma once
#include "GL/glew.h"
#include "foundation/math/vec4.h"
#include <memory>
#include "core/refcounted.h"
#include "core/windows/win32singleton.h"
#include "foundation/math/vec2.h"


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

#define MAX_KEYS 1024

enum MouseButton
{
	LEFT,
	MIDDLE,
	RIGHT
};

enum KeyCode
{
	// FUNCTION
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,

	// NUMBERS
	Alpha0 = 48,
	Alpha1 = 49,
	Alpha2 = 50,
	Alpha3 = 51,
	Alpha4 = 52,
	Alpha5 = 53,
	Alpha6 = 54,
	Alpha7 = 55,
	Alpha8 = 56,
	Alpha9 = 57,
	// NUMPAD
	Keypad0 = 320,
	Keypad1 = 321,
	Keypad2 = 322,
	Keypad3 = 323,
	Keypad4 = 324,
	Keypad5 = 325,
	Keypad6 = 326,
	Keypad7 = 327,
	Keypad8 = 328,
	Keypad9 = 329,
	// LETTERS
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Z = 89,
	Y = 90,
	// CONTROLS
	Esc = 256,
	Tab = 258,
	Shift_Left = 340,
	Shift_Right = 344,
	Ctrl_Left = 341, 
	Ctrl_Right = 345,
	Alt_Left = 342, 
	Alt_Right = 346,
	Space = 32,
	CapsLock = 280,
	Backspace = 259,
	Enter = 257,
	Delete = 261,
	Arrow_Left = 263,
	Arrow_Right = 262,
	Arrow_Up = 265,
	Arrow_Down = 264,
	Page_Up = 266,
	Page_Down = 267,
	Home = 268,
	End = 269,
	Insert = 260
};

class InputManager : public Core::RefCounted
{
__DeclareSingleton(InputManager)
public:
	~InputManager();

	void Setup(Display::Window* window, Render::LightNode* lNode);
	void Initialization();

	bool GetButtonKeyboard(const KeyCode key) { return keyboardButtons[key]; }
	bool GetButtonMouse(const MouseButton key) { return mouseButtons[key]; }
	Math::vec2 GetMousePosition() { return mousePosition; }

	void Update();

	Math::vec4 GetMovement();
	void CameraMovement();

private:

	Display::Window* window;

	GLfloat rotY = 0;
	GLfloat rotX = 0;
	GLfloat oldY = 0;
	GLfloat oldX = 0;
	GLfloat movSpeed = 0.4f;
	GLfloat movMultiplier = 3.5f;
	GLfloat scrollY = 0;
	GLfloat lightScroll = 0;
	GLfloat indexScroll = 0;
	GLboolean wireframe = false;

	Math::vec4 vec;
	float mouseX, mouseY;
	Math::vec2 mousePosition;

	Math::vec4 movement;

	Render::LightNode* lNode;

	static bool keyboardButtons[MAX_KEYS];
	static  bool mouseButtons[3];
	GLboolean wFlag, sFlag, dFlag, aFlag, shiftFlag, mLeftCheck, mRightCheck;
};

}
