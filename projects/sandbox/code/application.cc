//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "Application.h"
#include <chrono>
#include "foundation/math/mat4.h"
#include <assert.h>
#include "foundation/util/jsonparser.h"
#include "render/render/renderer.h"
#include "application/basegamefeatures/entitymanager.h"
#include "render/render/skybox.h"
#include "foundation/util/JobSystem.h"
#include "render/server/shaderserver.h"
#include "render/server/lightserver.h"
#include "application/game/entity.h"
#include "foundation/math/math.h"
#include "render/server/frameserver.h"
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

		Render::Renderer::Instance()->Setup(this->window);
		Render::Renderer::Instance()->SetWindowResolution(1280, 900);
		Render::Renderer::Instance()->SetRenderResolution(1280, 900);

		//shader = Render::ShaderServer::Instance()->GetShader("lightculling");

		Input::InputManager::Instance()->Setup(window, &lNode);

		Input::InputManager::Instance()->Initialization();
		Input::InputManager::Instance()->CameraReset();

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

	Math::mat4 modelMat = Math::mat4::translationMatrix(Math::vec4(0.0f, 0.0f, 0.0f));

	Ptr<Game::Entity> helmet = Game::Entity::Create();
	helmet->SetName("Helmet");
	helmet->SetModel("resources/models/scifihelmet.mdl");
	helmet->SetTransform(modelMat*Math::mat4::translationMatrix(Math::vec4(0.0f, 0.0f, 120.0f))*Math::mat4::rotY(Math::Deg2Rad(-180))*Math::mat4::Scale(30.0f));
	helmet->Activate();
	
	Ptr<Game::Entity> gun = Game::Entity::Create();
	gun->SetName("Gun");
	gun->SetModel("resources/models/cerberus.mdl");
	gun->SetTransform(modelMat*Math::mat4::translationMatrix(Math::vec4(0.0f, 50.0f, 0.0f))*Math::mat4::rotY(Math::Deg2Rad(-90))*Math::mat4::rotX(Math::Deg2Rad(-90)));
	gun->Activate();

	//terrain->SetSkybox(skybox);

    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();

	Render::LightServer::Instance()->CreatePointLight(Math::vec4(0, 80, 3), Math::vec4(1.0f, 1.0f, 1.0f), 10.0f, 10.0f);

	while (this->window->IsOpen())
	{        
		this->window->Update();
		Input::InputManager::Instance()->Update();
		
		//float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);

		Input::InputManager::Instance()->CameraMovement();

		//gN.setTransMat(modelMat);

		BaseGameFeature::EntityManager::Instance()->Update();

		//Render the scene
		Render::Renderer::Instance()->Render(false);
		
		//gN.draw();	

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->SwapBuffers();
	}
	this->window->Close();
}

	//------------------------------------------------------------------------------

} // namespace Example