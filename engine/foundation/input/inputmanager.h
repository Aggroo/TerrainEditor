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
enum MouseButton
{
	LEFT,
	MIDDLE,
	RIGHT
};

enum KeyCode
{
	// FUNCTION
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
	// NUMBERS
	Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,
	// NUMPAD
	Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
	// LETTERS
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	// CONTROLS
	Esc,
	Tab,
	Shift_Left, Shift_Right,
	Ctrl_Left, Ctrl_Right,
	Alt_Left, Alt_Right,
	Space,
	CapsLock,
	Backspace,
	Enter,
	Delete,
	Arrow_Left, Arrow_Right, Arrow_Up, Arrow_Down,
	Page_Up, Page_Down,
	Home,
	End,
	Insert
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

	void ReadKeyboard(int32 key, int32 action);
	void ReadMouse(int32 button, int32 action);

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

	bool keyboardButtons[83] = { false };
	bool mouseButtons[3] = { false };
	GLboolean wFlag, sFlag, dFlag, aFlag, shiftFlag, mLeftCheck, mRightCheck;
};

}
