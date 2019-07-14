//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "CGlab.h"
#include <chrono>
#include "foundation/math/mat4.h"
#include "application/game/terrain.h"
#include "render/render/renderer.h"
#include "application/basegamefeatures/entitymanager.h"
#include "render/render/skybox.h"
#include "render/server/shaderserver.h"
#include "render/server/lightserver.h"
#include "ui/uiserver.h"
#include "ui/widgets/widgettoolbar.h"
#include "ui/widgets/widgetinspector.h"
#include "ui/widgets/widgetmenubar.h"
#include "ui/widgets/widgetperlingenerator.h"
#include "ui/widgets/widgetterrainsettings.h"
#include "ui/widgets/widgetview.h"


using namespace Display;
namespace Application
{

//------------------------------------------------------------------------------
/**
*/
Application::Application()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
Application::~Application()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
Application::Open()
{
	App::Open();
	this->window = new Display::Window;

	if (this->window->Open())
	{
		shader = Render::ShaderObject::Create();

		GLuint comp = Render::ShaderServer::Instance()->LoadComputeShader("resources/shaders/lightculling.comp");

		shader->AddShader(comp);
		shader->LinkShaders();

		Render::ShaderServer::Instance()->AddShaderObject("LightCulling", shader);
		Render::Renderer::Instance()->Setup(this->window);
		Render::Renderer::Instance()->SetWindowResolution(1280, 900);
		Render::Renderer::Instance()->SetRenderResolution(1280, 900);
		terrain = TerrainEditor::Terrain::Create();
		terrain->Activate();

		
		Input::InputManager::Instance()->Setup(window, &lNode);

		Input::InputManager::Instance()->Initialization();

		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		//Enable backface culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		UI::UIServer::Instance()->Setup(this->window);
		UI::UIServer::Instance()->AddWidget(std::make_unique<UI::Toolbar>(this->window));
		UI::UIServer::Instance()->AddWidget(std::make_unique<UI::MenuBar>(this->window));
		UI::UIServer::Instance()->AddWidget(std::make_unique<UI::View>(this->window));
		UI::UIServer::Instance()->AddWidget(std::make_unique<UI::TerrainSettings>(this->window));
		UI::TerrainSettings* test = (UI::TerrainSettings*) UI::UIServer::Instance()->GetLastWidget();
		test->SetTerrain(terrain);
		UI::UIServer::Instance()->AddWidget(std::make_unique<UI::PerlinSettings>(this->window));
		UI::UIServer::Instance()->AddWidget(std::make_unique<UI::Inspector>(this->window));

		// set ui rendering function
		this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});
		
		return true;
	}
	return false;
}

void Application::RenderUI()
{
	if (this->window->IsOpen())
	{
		//Imgui code goes here!
		UI::UIServer::Instance()->Update();
	}
}

/**
*/
void
Application::Run()
{
	std::shared_ptr<Render::MeshResources> mesh = std::make_shared<Render::MeshResources>();
	std::shared_ptr<Render::TextureResource> tex = std::make_shared<Render::TextureResource>();
	
	//UI::UIServer::Instance()->SetTerrain(terrain);

	Math::mat4 modelMat = Math::mat4::translationMatrix(Math::vec4(0.0f, 0.0f, 0.0f));


	//lNode.setShaders(terrain->GetShader());
	//
	//lNode.setPos(0.0f, 250.0f, 0.0f);
	//lNode.setIntensity(0.6f, 0.6f, 0.6f);
	//lNode.setColour(0.2f, 0.2f, 0.2f);
    //lNode.apply();

	Ptr<Render::Skybox> skybox = Render::Skybox::Create();
	skybox->Activate();

    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();

	Render::LightServer::Instance()->CreatePointLight(Math::vec4(0, 800, 3), Math::vec4(0.3f, 0.3f, 0.3f), 10000.0f);

	while (this->window->IsOpen())
	{        
		this->window->Update();
		Input::InputManager::Instance()->Update();
		
		//float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);

		//gN.setTransMat(modelMat);
		//Render the scene
		Render::Renderer::Instance()->Render(false);
		
		//gN.draw();	

        glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->SwapBuffers();
	}
	this->window->Close();
}


	//------------------------------------------------------------------------------

} // namespace Example