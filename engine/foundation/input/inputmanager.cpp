#include "config.h"
#include "inputmanager.h"
#include "GLFW/glfw3.h"
#include "render/window.h"
#include "render/resources/shaderobject.h"
#include "render/resources/lightnode.h"
#include "render/render/renderer.h"
#include "render/camera/camera.h"
#include "render/resources/texturenode.h"



namespace Input
{

InputManager::InputManager() 
	: window(nullptr),
	mRightCheck(false), 
	mLeftCheck(false), 
	shiftFlag(false), 
	wFlag(false), 
	sFlag(false), 
	dFlag(false), 
	aFlag(false)
{
	vec = Math::vec4(0, 0, 0);
}

InputManager::~InputManager()
{
		
}

void InputManager::Setup(Display::Window* window, Render::LightNode* lNode)
{
	this->window = window;
	this->lNode = lNode;
	vec = Math::vec4(0, 0, 0);
}

void InputManager::Initialization()
{
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
		ReadKeyboard(key, action);

	});

	window->SetMouseMoveFunction([this](float64 x, float64 y)
	{
		mousePosition[0] = x;
		mousePosition[1] = y;

		if (GetButtonMouse(MouseButton::RIGHT))
		{
			rotX += x - oldX;
			rotY += y - oldY;
		}

		oldX = x;
		oldY = y;

	});

	window->SetMousePressFunction([this](int32 button, int32 action, int32 mods)
	{
		ReadMouse(button, action);
	});

	window->SetMouseScrollFunction([this](float64 x, float64 y)
	{
		if (mLeftCheck == true)
		{
			scrollY += y;
		}

	});

	window->SetWindowResizeFunction([this](int32 x, int32 y)
	{
		Render::Renderer::Instance()->SetRenderResolution(x, y);
		Render::Renderer::Instance()->SetWindowResolution(x, y);
	});
}

void InputManager::Update()
{

	movement = Math::vec4();
	if (GetButtonKeyboard(KeyCode::W))
	{
		if (GetButtonKeyboard(KeyCode::Shift_Left))
			movement[2] += movSpeed*movMultiplier;
		else
			movement[2] += movSpeed;
	}
	if (GetButtonKeyboard(KeyCode::S))
	{
		if (GetButtonKeyboard(KeyCode::Shift_Left))
			movement[2] -= movSpeed*movMultiplier;
		else
			movement[2] -= movSpeed;
	}
	if (GetButtonKeyboard(KeyCode::D))
	{
		if (GetButtonKeyboard(KeyCode::Shift_Left))
			movement[0] += movSpeed*movMultiplier;
		else
			movement[0] += movSpeed;
	}
	if (GetButtonKeyboard(KeyCode::A))
	{
		if (GetButtonKeyboard(KeyCode::Shift_Left))
			movement[0] -= movSpeed*movMultiplier;
		else
			movement[0] -= movSpeed;
	}

	if (GetButtonKeyboard(KeyCode::F2))
	{
		wireframe = !wireframe;
		wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
		
		

	if (GetButtonKeyboard(KeyCode::Esc))
		window->Close();

	CameraMovement();
}

Math::vec4 InputManager::GetMovement()
{
	return movement;
}

void InputManager::CameraMovement()
{
	mouseX = (oldX*2.0f) / 1024 - 1;
	mouseY = 1 - (oldY*2.0f) / 768;
	
	Math::mat4 rotx = Math::mat4::rotX(rotY*0.001f);
	Math::mat4 roty = Math::mat4::rotY(rotX*0.001f);

	Math::mat4 rotation = (roty*rotx);

	const Math::vec4 left = rotation.GetXAxis();
	const Math::vec4 up = rotation.GetYAxis();
	const Math::vec4 forward = rotation.GetZAxis();

	vec = vec + (rotation * movement);

	Graphics::MainCamera::Instance()->SetPosition(vec);

	Graphics::MainCamera::Instance()->LookAt(vec + forward, up);
}

void InputManager::ReadKeyboard(int32 key, int32 action)
{
	//Really should be handled nicer. Really "ugly" code
	if(action == GLFW_PRESS)
	{		
		switch (key)
		{
			// FUNCTION
			case GLFW_KEY_F1: keyboardButtons[0] = true; break;
			case GLFW_KEY_F2: keyboardButtons[1] = true; break;
			case GLFW_KEY_F3: keyboardButtons[2] = true; break;
			case GLFW_KEY_F4: keyboardButtons[3] = true; break;
			case GLFW_KEY_F5: keyboardButtons[4] = true; break;
			case GLFW_KEY_F6: keyboardButtons[5] = true; break;
			case GLFW_KEY_F7: keyboardButtons[6] = true; break;
			case GLFW_KEY_F8: keyboardButtons[7] = true; break;
			case GLFW_KEY_F9: keyboardButtons[8] = true; break;
			case GLFW_KEY_F10: keyboardButtons[9] = true; break;
			case GLFW_KEY_F11: keyboardButtons[10] = true; break;
			case GLFW_KEY_F12: keyboardButtons[11] = true; break;
			case GLFW_KEY_F13: keyboardButtons[12] = true; break;
			case GLFW_KEY_F14: keyboardButtons[13] = true; break;
			case GLFW_KEY_F15: keyboardButtons[14] = true; break;
			// NUMBERS
			case GLFW_KEY_0: keyboardButtons[15] = true; break;
			case GLFW_KEY_1: keyboardButtons[16] = true; break;
			case GLFW_KEY_2: keyboardButtons[17] = true; break;
			case GLFW_KEY_3: keyboardButtons[18] = true; break;
			case GLFW_KEY_4: keyboardButtons[19] = true; break;
			case GLFW_KEY_5: keyboardButtons[20] = true; break;
			case GLFW_KEY_6: keyboardButtons[21] = true; break;
			case GLFW_KEY_7: keyboardButtons[22] = true; break;
			case GLFW_KEY_8: keyboardButtons[23] = true; break;
			case GLFW_KEY_9: keyboardButtons[24] = true; break;
			// KEYPAD	
			case GLFW_KEY_KP_0: keyboardButtons[25] = true; break;
			case GLFW_KEY_KP_1: keyboardButtons[26] = true; break;
			case GLFW_KEY_KP_2: keyboardButtons[27] = true; break;
			case GLFW_KEY_KP_3: keyboardButtons[28] = true; break;
			case GLFW_KEY_KP_4: keyboardButtons[29] = true; break;
			case GLFW_KEY_KP_5: keyboardButtons[30] = true; break;
			case GLFW_KEY_KP_6: keyboardButtons[31] = true; break;
			case GLFW_KEY_KP_7: keyboardButtons[32] = true; break;
			case GLFW_KEY_KP_8: keyboardButtons[33] = true; break;
			case GLFW_KEY_KP_9: keyboardButtons[34] = true; break;
			// LETTERS	
			case GLFW_KEY_Q: keyboardButtons[35] = true; break;
			case GLFW_KEY_W: keyboardButtons[36] = true; break;
			case GLFW_KEY_E: keyboardButtons[37] = true; break;
			case GLFW_KEY_R: keyboardButtons[38] = true; break;
			case GLFW_KEY_T: keyboardButtons[39] = true; break;
			case GLFW_KEY_Y: keyboardButtons[40] = true; break;
			case GLFW_KEY_U: keyboardButtons[41] = true; break;
			case GLFW_KEY_I: keyboardButtons[42] = true; break;
			case GLFW_KEY_O: keyboardButtons[43] = true; break;
			case GLFW_KEY_P: keyboardButtons[44] = true; break;
			case GLFW_KEY_A: keyboardButtons[45] = true; break;
			case GLFW_KEY_S: keyboardButtons[46] = true; break;
			case GLFW_KEY_D: keyboardButtons[47] = true; break;
			case GLFW_KEY_F: keyboardButtons[48] = true; break;
			case GLFW_KEY_G: keyboardButtons[49] = true; break;
			case GLFW_KEY_H: keyboardButtons[50] = true; break;
			case GLFW_KEY_J: keyboardButtons[51] = true; break;
			case GLFW_KEY_K: keyboardButtons[52] = true; break;
			case GLFW_KEY_L: keyboardButtons[53] = true; break;
			case GLFW_KEY_Z: keyboardButtons[54] = true; break;
			case GLFW_KEY_X: keyboardButtons[55] = true; break;
			case GLFW_KEY_C: keyboardButtons[56] = true; break;
			case GLFW_KEY_V: keyboardButtons[57] = true; break;
			case GLFW_KEY_B: keyboardButtons[58] = true; break;
			case GLFW_KEY_N: keyboardButtons[59] = true; break;
			case GLFW_KEY_M: keyboardButtons[60] = true; break;
			// CONTROLS		
			case GLFW_KEY_ESCAPE: keyboardButtons[61] = true; break;
			case GLFW_KEY_TAB: keyboardButtons[62] = true; break;
			case GLFW_KEY_LEFT_SHIFT: keyboardButtons[63] = true; break;
			case GLFW_KEY_RIGHT_SHIFT: keyboardButtons[64] = true; break;
			case GLFW_KEY_LEFT_CONTROL: keyboardButtons[65] = true; break;
			case GLFW_KEY_RIGHT_CONTROL: keyboardButtons[66] = true; break;
			case GLFW_KEY_LEFT_ALT: keyboardButtons[67] = true; break;
			case GLFW_KEY_RIGHT_ALT: keyboardButtons[68] = true; break;
			case GLFW_KEY_SPACE: keyboardButtons[69] = true; break;
			case GLFW_KEY_CAPS_LOCK: keyboardButtons[70] = true; break;
			case GLFW_KEY_BACKSPACE: keyboardButtons[71] = true; break;
			case GLFW_KEY_ENTER: keyboardButtons[72] = true; break;
			case GLFW_KEY_DELETE: keyboardButtons[73] = true; break;
			case GLFW_KEY_LEFT: keyboardButtons[74] = true; break;
			case GLFW_KEY_RIGHT: keyboardButtons[75] = true; break;
			case GLFW_KEY_UP: keyboardButtons[76] = true; break;
			case GLFW_KEY_DOWN: keyboardButtons[77] = true; break;
			case GLFW_KEY_PAGE_UP: keyboardButtons[78] = true; break;
			case GLFW_KEY_PAGE_DOWN: keyboardButtons[79] = true; break;
			case GLFW_KEY_HOME: keyboardButtons[80] = true; break;
			case GLFW_KEY_END: keyboardButtons[81] = true; break;
			case GLFW_KEY_INSERT: keyboardButtons[82] = true; break;

		} 
	}
	else if(action == GLFW_RELEASE)
	{
		switch (key)
		{
			// FUNCTION
		case GLFW_KEY_F1: keyboardButtons[0] = false; break;
		case GLFW_KEY_F2: keyboardButtons[1] = false; break;
		case GLFW_KEY_F3: keyboardButtons[2] = false; break;
		case GLFW_KEY_F4: keyboardButtons[3] = false; break;
		case GLFW_KEY_F5: keyboardButtons[4] = false; break;
		case GLFW_KEY_F6: keyboardButtons[5] = false; break;
		case GLFW_KEY_F7: keyboardButtons[6] = false; break;
		case GLFW_KEY_F8: keyboardButtons[7] = false; break;
		case GLFW_KEY_F9: keyboardButtons[8] = false; break;
		case GLFW_KEY_F10: keyboardButtons[9] = false; break;
		case GLFW_KEY_F11: keyboardButtons[10] = false; break;
		case GLFW_KEY_F12: keyboardButtons[11] = false; break;
		case GLFW_KEY_F13: keyboardButtons[12] = false; break;
		case GLFW_KEY_F14: keyboardButtons[13] = false; break;
		case GLFW_KEY_F15: keyboardButtons[14] = false; break;
			// NUMBERS
		case GLFW_KEY_0: keyboardButtons[15] = false; break;
		case GLFW_KEY_1: keyboardButtons[16] = false; break;
		case GLFW_KEY_2: keyboardButtons[17] = false; break;
		case GLFW_KEY_3: keyboardButtons[18] = false; break;
		case GLFW_KEY_4: keyboardButtons[19] = false; break;
		case GLFW_KEY_5: keyboardButtons[20] = false; break;
		case GLFW_KEY_6: keyboardButtons[21] = false; break;
		case GLFW_KEY_7: keyboardButtons[22] = false; break;
		case GLFW_KEY_8: keyboardButtons[23] = false; break;
		case GLFW_KEY_9: keyboardButtons[24] = false; break;
			// KEYPAD	
		case GLFW_KEY_KP_0: keyboardButtons[25] = false; break;
		case GLFW_KEY_KP_1: keyboardButtons[26] = false; break;
		case GLFW_KEY_KP_2: keyboardButtons[27] = false; break;
		case GLFW_KEY_KP_3: keyboardButtons[28] = false; break;
		case GLFW_KEY_KP_4: keyboardButtons[29] = false; break;
		case GLFW_KEY_KP_5: keyboardButtons[30] = false; break;
		case GLFW_KEY_KP_6: keyboardButtons[31] = false; break;
		case GLFW_KEY_KP_7: keyboardButtons[32] = false; break;
		case GLFW_KEY_KP_8: keyboardButtons[33] = false; break;
		case GLFW_KEY_KP_9: keyboardButtons[34] = false; break;
			// LETTERS	
		case GLFW_KEY_Q: keyboardButtons[35] = false; break;
		case GLFW_KEY_W: keyboardButtons[36] = false; break;
		case GLFW_KEY_E: keyboardButtons[37] = false; break;
		case GLFW_KEY_R: keyboardButtons[38] = false; break;
		case GLFW_KEY_T: keyboardButtons[39] = false; break;
		case GLFW_KEY_Y: keyboardButtons[40] = false; break;
		case GLFW_KEY_U: keyboardButtons[41] = false; break;
		case GLFW_KEY_I: keyboardButtons[42] = false; break;
		case GLFW_KEY_O: keyboardButtons[43] = false; break;
		case GLFW_KEY_P: keyboardButtons[44] = false; break;
		case GLFW_KEY_A: keyboardButtons[45] = false; break;
		case GLFW_KEY_S: keyboardButtons[46] = false; break;
		case GLFW_KEY_D: keyboardButtons[47] = false; break;
		case GLFW_KEY_F: keyboardButtons[48] = false; break;
		case GLFW_KEY_G: keyboardButtons[49] = false; break;
		case GLFW_KEY_H: keyboardButtons[50] = false; break;
		case GLFW_KEY_J: keyboardButtons[51] = false; break;
		case GLFW_KEY_K: keyboardButtons[52] = false; break;
		case GLFW_KEY_L: keyboardButtons[53] = false; break;
		case GLFW_KEY_Z: keyboardButtons[54] = false; break;
		case GLFW_KEY_X: keyboardButtons[55] = false; break;
		case GLFW_KEY_C: keyboardButtons[56] = false; break;
		case GLFW_KEY_V: keyboardButtons[57] = false; break;
		case GLFW_KEY_B: keyboardButtons[58] = false; break;
		case GLFW_KEY_N: keyboardButtons[59] = false; break;
		case GLFW_KEY_M: keyboardButtons[60] = false; break;
			// CONTROLS		
		case GLFW_KEY_ESCAPE: keyboardButtons[61] = false; break;
		case GLFW_KEY_TAB: keyboardButtons[62] = false; break;
		case GLFW_KEY_LEFT_SHIFT: keyboardButtons[63] = false; break;
		case GLFW_KEY_RIGHT_SHIFT: keyboardButtons[64] = false; break;
		case GLFW_KEY_LEFT_CONTROL: keyboardButtons[65] = false; break;
		case GLFW_KEY_RIGHT_CONTROL: keyboardButtons[66] = false; break;
		case GLFW_KEY_LEFT_ALT: keyboardButtons[67] = false; break;
		case GLFW_KEY_RIGHT_ALT: keyboardButtons[68] = false; break;
		case GLFW_KEY_SPACE: keyboardButtons[69] = false; break;
		case GLFW_KEY_CAPS_LOCK: keyboardButtons[70] = false; break;
		case GLFW_KEY_BACKSPACE: keyboardButtons[71] = false; break;
		case GLFW_KEY_ENTER: keyboardButtons[72] = false; break;
		case GLFW_KEY_DELETE: keyboardButtons[73] = false; break;
		case GLFW_KEY_LEFT: keyboardButtons[74] = false; break;
		case GLFW_KEY_RIGHT: keyboardButtons[75] = false; break;
		case GLFW_KEY_UP: keyboardButtons[76] = false; break;
		case GLFW_KEY_DOWN: keyboardButtons[77] = false; break;
		case GLFW_KEY_PAGE_UP: keyboardButtons[78] = false; break;
		case GLFW_KEY_PAGE_DOWN: keyboardButtons[79] = false; break;
		case GLFW_KEY_HOME: keyboardButtons[80] = false; break;
		case GLFW_KEY_END: keyboardButtons[81] = false; break;
		case GLFW_KEY_INSERT: keyboardButtons[82] = false; break;

		}
	}
}

void InputManager::ReadMouse(int32 button, int32 action)
{
	if(action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT: mouseButtons[0] = true; break;
		case GLFW_MOUSE_BUTTON_MIDDLE: mouseButtons[1] = true; break;
		case GLFW_MOUSE_BUTTON_RIGHT: mouseButtons[2] = true; break;
		}
	}
	else if(action == GLFW_RELEASE)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT: mouseButtons[0] = false; break;
		case GLFW_MOUSE_BUTTON_MIDDLE: mouseButtons[1] = false; break;
		case GLFW_MOUSE_BUTTON_RIGHT: mouseButtons[2] = false; break;
		}
	}
}
}
