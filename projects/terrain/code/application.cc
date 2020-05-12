//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
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
#include "ui/widgets/widgetrendersettings.h"


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
		JobSystem::Initialize();

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
		UI::UIServer::Instance()->AddWidget(std::make_unique<UI::RenderSettings>(this->window));
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


bool Application::IsInitializationFinished()
{
	return initializeStarted && !JobSystem::IsBusy(ctx);
}

/**
*/
void
Application::Run()
{
	Render::LightServer::Instance()->CreatePointLight(Math::vec4(0, 80, 3), Math::vec4(1.0f, 1.0f, 1.0f), 10.0f, 10.0f);
	while (this->window->IsOpen())
	{
		if(!IsInitializationFinished())
			continue;

		this->window->Update();
		Input::InputManager::Instance()->Update();
		
		BaseGameFeature::EntityManager::Instance()->Update();

		//Render the scene
		Render::Renderer::Instance()->Render(false);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->SwapBuffers();
	}
	this->window->Close();
}


	//------------------------------------------------------------------------------

} // namespace Example