#include "config.h"
#include "inputmanager.h"
#include "GLFW/glfw3.h"
#include "render/window.h"
#include "shaderobject.h"
#include "lightnode.h"
#include "render/render/renderer.h"
#include "render/camera/camera.h"

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
		
}

InputManager::InputManager(Display::Window* window, Math::ShaderObject* shaders, Math::LightNode* lNode) 
	: mRightCheck(false), 
	mLeftCheck(false), 
	shiftFlag(false), 
	wFlag(false), 
	sFlag(false), 
	dFlag(false), 
	aFlag(false)
{
	this->window = window;
	this->shaders = shaders;
	this->lNode = lNode;
}

InputManager::~InputManager()
{
		
}

void InputManager::InitKeyPress()
{
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
		if (mRightCheck == true)
		{
			if (key == GLFW_KEY_W && action == GLFW_PRESS)
			{
				wFlag = true;
			}
			else if (key == GLFW_KEY_S && action == GLFW_PRESS)
			{
				sFlag = true;
			}
			else if (key == GLFW_KEY_D && action == GLFW_PRESS)
			{
				dFlag = true;
			}
			else if (key == GLFW_KEY_A && action == GLFW_PRESS)
			{
				aFlag = true;
			}
			else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
			{
				shiftFlag = true;
			}

		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			wFlag = false;
		}
		else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			sFlag = false;
		}
		else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			dFlag = false;
		}
		else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			aFlag = false;
		}
		else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		{
			shiftFlag = false;
		}
		else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			this->window->Close();
		}
		else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		{
			shaders->ReloadShaders();
			shaders->setupVector3f("u_matAmbientReflectance", 1.0f, 1.0f, 1.0f);
			shaders->setupVector3f("u_matDiffuseReflectance", 1.0f, 1.0f, 1.0f);
			shaders->setupVector3f("u_matSpecularReflectance", 1.0f, 1.0f, 1.0f);
			shaders->setupUniformFloat("u_matShininess", 64.0f);

			shaders->setupUniformInt("textures[0]", 0);
			shaders->setupUniformInt("textures[1]", 1);
			shaders->setupUniformInt("textures[2]", 2);
			shaders->setupUniformInt("textures[3]", 3);
			shaders->setupUniformInt("pathtex", 4);
			shaders->setupUniformInt("textures[4]", 5);

			lNode->apply();
		}

	});
}

void InputManager::InitMouse()
{

	window->SetMouseMoveFunction([this](float64 x, float64 y)
	{
		if (mRightCheck)
		{
			rotX += x - oldX;
			rotY += y - oldY;
		}

		oldX = x;
		oldY = y;

	});

	window->SetMousePressFunction([this](int32 button, int32 action, int32 mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mLeftCheck = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			mLeftCheck = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			mRightCheck = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			mRightCheck = false;
		}
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
		Graphics::MainCamera::Instance()->UpdateProjectionMatrix();
	});
}

void InputManager::Update()
{

	movement = Math::vec4();
	if (wFlag == true)
	{
		if (shiftFlag)
			movement[2] += movSpeed*movMultiplier;
		else
			movement[2] += movSpeed;
	}
	if (sFlag == true)
	{
		if (shiftFlag)
			movement[2] -= movSpeed*movMultiplier;
		else
			movement[2] -= movSpeed;
	}
	if (dFlag == true)
	{
		if (shiftFlag)
			movement[0] -= movSpeed*movMultiplier;
		else
			movement[0] -= movSpeed;
	}
	if (aFlag == true)
	{
		if (shiftFlag)
			movement[0] += movSpeed*movMultiplier;
		else
			movement[0] += movSpeed;
	}
}

	GLfloat InputManager::GetRotX()
{
	return rotX;
}

GLfloat InputManager::GetRotY()
{
	return rotY;
}

GLfloat InputManager::GetOldX()
{
	return oldX;
}

GLfloat InputManager::GetOldY()
{
	return oldY;
}

Math::vec4 InputManager::GetMovement()
{
	return movement;
}

}
