//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "Application.h"
#include <chrono>
#include "foundation/math/mat4.h"
#include <assert.h>
#include "UserInterface.h"
#include "foundation/util/JsonParser.h"
#include "render/render/renderer.h"
#include "application/basegamefeatures/entitymanager.h"
#include "render/render/skybox.h"
#include "foundation/util/threadpool.h"
#include "render/server/shaderserver.h"
#include "render/server/lightserver.h"
#include "application/game/entity.h"
#include "foundation/math/math.h"
#include "render/server/frameserver.h"

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
	sphere->SetTextures("resources/textures/terrain_textures/mossy-ground/mixedmoss-albedo2.png",
						"resources/textures/terrain_textures/mossy-ground/mixedmoss-normal2.png",
						"resources/textures/terrain_textures/mossy-ground/mixedmoss-metalness.png",
						"resources/textures/terrain_textures/mossy-ground/mixedmoss-roughness.png");
	sphere->SetShaders("resources/shaders/PBR.vert", "resources/shaders/PBR.frag", "PBR");
	sphere->SetIBLMaps(skybox->GetCubemap(), Render::FrameServer::Instance()->GetIBLPass()->GetIrradianceMap(), Render::FrameServer::Instance()->GetIBLPass()->GetBRDFMap());
	sphere->SetTransform(modelMat*Math::mat4::translationMatrix(Math::vec4(-50.0f, 0.0f, 0.0f)));
	sphere->Activate();

	Ptr<Game::Entity> teapot = Game::Entity::Create();
	teapot->SetName("Teapot");
	teapot->SetMesh("resources/models/HelmetPresentationLightMap.fbx.fbx");
	teapot->SetTextures("resources/textures/helmet/BaseColor.png",
						"resources/textures/helmet/NormalMap.png",
						"resources/textures/helmet/Metalness.png",
						"resources/textures/helmet/Roughness.png",
						"resources/textures/helmet/AOMap.png");
	teapot->SetShaders("resources/shaders/PBR.vert", "resources/shaders/PBR.frag", "PBR");
	teapot->SetIBLMaps(skybox->GetCubemap(), Render::FrameServer::Instance()->GetIBLPass()->GetIrradianceMap(), Render::FrameServer::Instance()->GetIBLPass()->GetBRDFMap());
	teapot->SetTransform(modelMat*Math::mat4::translationMatrix(Math::vec4(-120.0f, 0.0f, 0.0f))*Math::mat4::vectorScaling(10.0f, 10.0f, 10.0f));
	teapot->Activate();

	Ptr<Game::Entity> gun = Game::Entity::Create();
	gun->SetName("Gun");
	gun->SetMesh("resources/models/Cerberus_LP.FBX");
	gun->SetTextures("resources/textures/cerberus/Cerberus_A.tga",
					 "resources/textures/cerberus/Cerberus_N.tga",
					 "resources/textures/cerberus/Cerberus_M.tga",
					 "resources/textures/cerberus/Cerberus_R.tga",
					 "resources/textures/cerberus/Cerberus_AO.tga");
	gun->SetShaders("resources/shaders/PBR.vert", "resources/shaders/PBR.frag", "PBR");
	gun->SetIBLMaps(skybox->GetCubemap(), Render::FrameServer::Instance()->GetIBLPass()->GetIrradianceMap(), Render::FrameServer::Instance()->GetIBLPass()->GetBRDFMap());
	gun->SetTransform(modelMat*Math::mat4::translationMatrix(Math::vec4(0.0f, -20.0f, 50.0f))*Math::mat4::rotY(Math::Deg2Rad(-90))*Math::mat4::rotX(Math::Deg2Rad(-90)));
	gun->Activate();

	lNode.setShaders(Render::ShaderServer::Instance()->GetShader("PBR"));
	
	lNode.setPos(0.0f, 250.0f, 0.0f);
	lNode.setIntensity(0.6f, 0.6f, 0.6f);
	lNode.setColour(0.2f, 0.2f, 0.2f);
    lNode.apply();


	//terrain->SetSkybox(skybox);

    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();

	//LightServer::Instance()->CreatePointLight(Math::vec4(0, 800, 3), Math::vec4(0.3f, 0.3f, 0.3f), 10.0f);

	while (this->window->IsOpen() && !this->shutdown)
	{        
		this->window->Update();
		Input::InputManager::Instance()->Update();
		
		//float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);

		Input::InputManager::Instance()->CameraMovement();

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