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
bool InputManager::keyboardButtons[MAX_KEYS];
bool InputManager::mouseButtons[3];

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
		keyboardButtons[key] = action != GLFW_RELEASE;

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
