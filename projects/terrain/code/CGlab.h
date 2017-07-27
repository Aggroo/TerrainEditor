//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#pragma once
#include "core/app.h"
#include "render/window.h"
#include "render/resources/graphicsnode.h"
#include "render/resources/lightnode.h"
#include "render/resources/meshresource.h"
#include "render/resources/textureresource.h"
#include "render/resources/shaderobject.h"
#include <chrono>

namespace Example
{
class CGLab : public Core::App
{
public:
	/// constructor
	CGLab();
	/// destructor
	~CGLab();

	/// open app
	bool Open();
	/// run app
	void Run();
private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	CGMath::vector4D vec;
	GLfloat rotY = 0;
	GLfloat rotX = 0;
	GLfloat oldY = 0;
	GLfloat oldX = 0;
	GLfloat movSpeed = 1.0f;
	GLfloat movMultiplier = 3.5f;
	GLfloat scrollY = 0;
	GLfloat lightScroll = 0;
	GLfloat indexScroll = 0;
	GLboolean wFlag, sFlag, dFlag, aFlag, shiftFlag, mLeftCheck, mRightCheck;

	Display::Window* window;
	CGMath::GraphicsNode gN;
	CGMath::LightNode lNode;

	CGMath::TextureNode textures;
	std::shared_ptr<CGMath::ShaderObject> shaders;
	float mouseX, mouseY;
	
	//GLuint shaderTransMatrix;
	

};
} // namespace Example