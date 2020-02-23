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
#include "foundation/input/inputmanager.h"
#include <chrono>

namespace Application
{
class Application : public Core::App
{
public:
	/// constructor
	Application();
	/// destructor
	~Application();

	/// open app
	bool Open();
	void RenderUI();
	/// run app
	void Run();

	Display::Window* GetWindow(){ return window; }

private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	Math::vec4 vec;

	Display::Window* window;
	Render::LightNode lNode;

	Ptr<TerrainEditor::Terrain> terrain;
	Ptr<Render::ShaderObject> shader;

	//GLuint shaderTransMatrix;
	

};
} // namespace Example