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
#include "render/resources/shaderobject.h"
#include "render/resources/inputmanager.h"
#include "render/resources/deferredrenderer.h"
#include <chrono>

class UserInterface;

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
	void RenderUI();
	/// run app
	void Run();

	Display::Window* GetWindow(){ return window; }
private:

	std::shared_ptr<UserInterface> UI;

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	CGMath::vector4D vec;

	Display::Window* window;
	CGMath::GraphicsNode gN;
	CGMath::LightNode lNode;

	CGMath::TextureNode textures;
	std::shared_ptr<Input::InputManager> input;
	std::shared_ptr<CGMath::ShaderObject> shaders;
	std::shared_ptr<Renderer::DeferredRenderer> render;
	float mouseX, mouseY;
	
	//GLuint shaderTransMatrix;
	

};
} // namespace Example