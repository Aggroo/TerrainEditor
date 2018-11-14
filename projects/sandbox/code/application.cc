//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "Application.h"
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
#include "foundation/util/threadpool.h"
#include "render/server/shaderserver.h"
#include "render/server/lightserver.h"
#include "application/game/entity.h"

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
Application::Application() : shutdown(false)
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

		input = std::make_shared<Input::InputManager>(this->window, &lNode);

		input->InitKeyPress();
		input->InitMouse();

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

void Application::RenderUI()
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
Application::Run()
{

	Math::mat4 modelMat = Math::mat4::translationMatrix(Math::vec4(0.0f, 0.0f, 0.0f));
	//TODO: Fix IBL pipeline with prefilter map, irradience map and BRDF map
	Ptr<Render::Skybox> skybox = Render::Skybox::Create();
	skybox->Activate();

	Ptr<Game::Entity> sphere = Game::Entity::Create();
	sphere->SetName("Sphere");
	sphere->SetMesh("resources/models/sphere.obj");
	sphere->SetTextures("resources/textures/terrain_textures/GroundForestRoots001/GroundForestRoots001_ALBEDO_1K.jpg",
						"resources/textures/terrain_textures/GroundForestRoots001/GroundForestRoots001_NRM_1K.jpg",
						"resources/textures/terrain_textures/GroundForestRoots001/GroundForestRoots001_GLOSS_1K.jpg",
						"resources/textures/terrain_textures/GroundForestRoots001/GroundForestRoots001_REFL_1K.jpg",
						"resources/textures/terrain_textures/GroundForestRoots001/GroundForestRoots001_AO_1K.jpg");
	sphere->SetShaders("resources/shaders/PBR.vert", "resources/shaders/PBR.frag", "PBR");
	sphere->SetEnvironmentMap(skybox->GetCubemap());
	sphere->SetTransform(modelMat);
	sphere->Activate();

	Ptr<Game::Entity> teapot = Game::Entity::Create();
	teapot->SetName("Teapot");
	teapot->SetMesh("resources/models/sphere.obj");
	teapot->SetTextures("resources/textures/terrain_textures/MetalSpottyDiscoloration001/MetalSpottyDiscoloration001_COL_1K_METALNESS.jpg",
						"resources/textures/terrain_textures/MetalSpottyDiscoloration001/MetalSpottyDiscoloration001_NRM_1K_METALNESS.jpg",
						"resources/textures/terrain_textures/MetalSpottyDiscoloration001/MetalSpottyDiscoloration001_METALNESS_1K_METALNESS.jpg",
						"resources/textures/terrain_textures/MetalSpottyDiscoloration001/MetalSpottyDiscoloration001_ROUGHNESS_1K_METALNESS.jpg");
	teapot->SetShaders("resources/shaders/PBR.vert", "resources/shaders/PBR.frag", "PBR");
	teapot->SetEnvironmentMap(skybox->GetCubemap());
	teapot->SetTransform(modelMat*Math::mat4::translationMatrix(Math::vec4(50.0f, 0.0f, 0.0f)));
	teapot->Activate();

	lNode.setShaders(sphere->GetShader());
	
	lNode.setPos(0.0f, 250.0f, 0.0f);
	lNode.setIntensity(0.6f, 0.6f, 0.6f);
	lNode.setColour(0.2f, 0.2f, 0.2f);
    lNode.apply();


	//terrain->SetSkybox(skybox);

    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();

	LightServer::Instance()->CreatePointLight(Math::vec4(0, 800, 3), Math::vec4(0.3f, 0.3f, 0.3f), 10000.0f);

	while (this->window->IsOpen() && !this->shutdown)
	{        
		this->window->Update();
		input->Update();
		
		//float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);

		input->CameraMovement();

		//gN.setTransMat(modelMat);
		//Render the scene
		Render::Renderer::Instance()->Render(false);
		
		//gN.draw();	

        glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->SwapBuffers();
	}
	this->window->Close();
}

void Application::Shutdown(bool shutdown)
{
	this->shutdown = shutdown;
}


	//------------------------------------------------------------------------------

} // namespace Example