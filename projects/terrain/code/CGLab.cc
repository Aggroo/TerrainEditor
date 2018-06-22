//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "CGlab.h"
#include <chrono>
#include "foundation/math/mat4.h"
#include <assert.h>
#include "render/resources/HalfEdgeMesh.h"
#include "application/game/terrain.h"
#include "imguiextra.h"
#include "UserInterface.h"
#include "foundation/util/JsonParser.h"
#include "render/camera/camera.h"
#include "render/render/renderer.h"
#include "application/basegamefeatures/entitymanager.h"
#include "render/render/skybox.h"

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
CGLab::CGLab() : shutdown(false)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
CGLab::~CGLab()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
CGLab::Open()
{
	App::Open();
	this->window = new Display::Window;

	if (this->window->Open())
	{
		Render::Renderer::Instance()->Setup(this->window);

		terrain = TerrainEditor::Terrain::Create();
		terrain->Activate();

		shaders = std::make_shared<Math::ShaderObject>();		
		input = std::make_shared<Input::InputManager>(this->window, terrain->GetShader(), &lNode);

		input->InitKeyPress();
		input->InitMouse();

		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glEnable(GL_MULTISAMPLE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		//Enable backface culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		this->UI = std::make_shared<UserInterface>(this);

		// set ui rendering function
		this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});
		
		return true;
	}
	return false;
}

void CGLab::RenderUI()
{
	if (this->window->IsOpen())
	{
		//Imgui code goes here!
		UI->Run();
	}
}

/**
*/
void
CGLab::Run()
{

	std::shared_ptr<Math::MeshResources> mesh = std::make_shared<Math::MeshResources>();
	std::shared_ptr<Math::TextureResource> tex = std::make_shared<Math::TextureResource>();
	Ptr<Render::Skybox> skybox = Render::Skybox::Create();
	skybox->Activate();
	UI->SetTerrain(terrain);

	mat4 modelMat = mat4::translationMatrix(Math::vec4(0.0f, 0.0f, 0.0f));

	lNode.setShaders(terrain->GetShader());
	
	lNode.setPos(0.0f, 250.0f, 0.0f);
	lNode.setIntensity(0.6f, 0.6f, 0.6f);
	lNode.setColour(0.2f, 0.2f, 0.2f);
    lNode.apply();


    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();

	while (this->window->IsOpen() && !this->shutdown)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		this->window->Update();
		input->Update();
		
		//float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);

		input->CameraMovement();

		//gN.setTransMat(modelMat);
		
		BaseGameFeature::EntityManager::Instance()->Update();
		//gN.draw();	

        glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->SwapBuffers();
	}
	this->window->Close();
}

	void CGLab::Shutdown(bool shutdown)
{
	this->shutdown = shutdown;
}


	//------------------------------------------------------------------------------

} // namespace Example