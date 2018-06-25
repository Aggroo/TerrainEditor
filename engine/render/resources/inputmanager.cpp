#include "config.h"
#include "inputmanager.h"
#include "GLFW/glfw3.h"
#include "render/window.h"
#include "shaderobject.h"
#include "lightnode.h"
#include "render/render/renderer.h"
#include "render/camera/camera.h"
#include "texturenode.h"

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
	vec = Math::vec4(0, 0, 0);
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
			shaders->setupVector3f("u_matSpecularReflectance", 0.16f, 0.16f, 0.16f);
			shaders->setupUniformFloat("u_matShininess", 12.0f);

			shaders->setupUniformInt("textures[0]",(GLuint) Render::TextureIndex::albedo0);
			shaders->setupUniformInt("textures[1]",(GLuint) Render::TextureIndex::albedo1);
			shaders->setupUniformInt("textures[2]",(GLuint) Render::TextureIndex::albedo2);

			shaders->setupUniformInt("normals[0]", (GLuint) Render::TextureIndex::normal0);
			shaders->setupUniformInt("normals[1]", (GLuint) Render::TextureIndex::normal1);
			shaders->setupUniformInt("normals[2]", (GLuint) Render::TextureIndex::normal2);

			shaders->setupUniformInt("splat", (GLuint)Render::TextureIndex::splat);

			shaders->setupUniformFloat("tex0UvMultiplier", 0.1f);
			shaders->setupUniformFloat("tex1UvMultiplier", 0.1f);
			shaders->setupUniformFloat("tex2UvMultiplier", 0.1f);

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
		Render::Renderer::Instance()->SetWindowResolution(x, y);
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
			movement[0] += movSpeed*movMultiplier;
		else
			movement[0] += movSpeed;
	}
	if (aFlag == true)
	{
		if (shiftFlag)
			movement[0] -= movSpeed*movMultiplier;
		else
			movement[0] -= movSpeed;
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
}
