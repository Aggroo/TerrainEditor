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
#include <chrono>
#include "UserInterface.h"

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

	void Shutdown(bool shutdown);
private:

	std::shared_ptr<UserInterface> UI;

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	Math::vec4 vec;

	Display::Window* window;
	Math::GraphicsNode gN;
	Math::LightNode lNode;

	std::shared_ptr<Input::InputManager> input;
	std::shared_ptr<Math::ShaderObject> shaders;
	Ptr<TerrainEditor::Terrain> terrain;

	

	bool shutdown;
	
	//GLuint shaderTransMatrix;
	

};
} // namespace Example