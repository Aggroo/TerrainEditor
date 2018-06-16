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

		shaders = std::make_shared<Math::ShaderObject>();		
		input = std::make_shared<Input::InputManager>(this->window, shaders.get(), &lNode);

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
	std::shared_ptr<TerrainEditor::Terrain> terrain = std::make_shared<TerrainEditor::Terrain>();
	terrain->Activate();
	vec = Math::vec4(0, 0, 0);

	UI->SetTerrain(terrain);

	mat4 modelMat = mat4::translationMatrix(Math::vec4(0.0f, 0.0f, 0.0f));

	//terrain->CreateTerrain("resources/textures/perlin2.jpg", 3.0f, 100.0f);

	//mesh->createQuad();
	//mesh->loadObj("resources/models/ARC170.obj");

	//mesh->genBuffer();
		
	//shaders->setupShaders("resources/shaders/Blinn-phong.vert", "resources/shaders/Blinn-phong.frag");

	lNode.setShaders(terrain->GetShader());
	
	lNode.setPos(0.0f, 250.0f, 0.0f);
	lNode.setIntensity(0.6f, 0.6f, 0.6f);
	lNode.setColour(0.2f, 0.2f, 0.2f);
    lNode.apply();

    //shaders->setupVector3f("u_matAmbientReflectance", 1.0f, 1.0f, 1.0f);
    //shaders->setupVector3f("u_matDiffuseReflectance", 1.0f, 1.0f, 1.0f);
    //shaders->setupVector3f("u_matSpecularReflectance", 1.0f, 1.0f, 1.0f);
    //shaders->setupUniformFloat("u_matShininess", 64.0f);
	///*shaders->setupMatrix4fv("transMatrix", modelMat);*/
	//
	//textures.AddTexture("resources/textures/water.jpg");
	//textures.AddTexture("resources/textures/sand.jpg");
	//textures.AddTexture("resources/textures/grass.jpg");
	//textures.AddTexture("resources/textures/rock.jpg");
	//
	//textures.AddTexture("resources/textures/pathway.jpg");
	//textures.AddTexture("resources/textures/road.jpg");
	//
	//shaders->setupUniformInt("textures[0]", 0);
	//shaders->setupUniformInt("textures[1]", 1);
	//shaders->setupUniformInt("textures[2]", 2);
	//shaders->setupUniformInt("textures[3]", 3);
	//shaders->setupUniformInt("textures[4]", 5);
	//shaders->setupUniformInt("pathtex", 4);

	//gN.setMesh(terrain->GetMesh());
	//gN.setShaders(shaders);
	//gN.setTex(std::make_shared<Math::TextureNode>(textures));

    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();

	while (this->window->IsOpen() && !this->shutdown)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		this->window->Update();
		input->Update();
		
		//float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);

		CameraMovement(terrain);

		//gN.setTransMat(modelMat);
		Render::Renderer::Instance()->SetupUniformBuffer(Graphics::MainCamera::Instance());
		BaseGameFeature::EntityManager::Instance()->Update();
		//gN.draw();	

        glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->SwapBuffers();
	}
	this->window->Close();
}

void CGLab::CameraMovement(std::shared_ptr<TerrainEditor::Terrain> terrain)
{
	mouseX = (input->GetOldX()*2.0f) / 1024 - 1;
	mouseY = 1 - (input->GetOldY()*2.0f) / 768;

	mat4 rotx = mat4::rotX(-input->GetRotY()*0.001f);
	mat4 roty = mat4::rotY(input->GetRotX()*0.001f);

	mat4 rotation = (roty*rotx);

	const Math::vec4 left = rotation.GetXAxis();
	const Math::vec4 up = rotation.GetYAxis();
	const Math::vec4 forward = rotation.GetZAxis();

	vec = vec + (rotation * input->GetMovement());

	Graphics::MainCamera::Instance()->SetPosition(vec);

	Graphics::MainCamera::Instance()->LookAt(vec + forward, up);

	terrain->GetShader()->setupVector3f("cameraPosition", Graphics::MainCamera::Instance()->GetCameraPosition());
}

	void CGLab::Shutdown(bool shutdown)
{
	this->shutdown = shutdown;
}


	//------------------------------------------------------------------------------

} // namespace Example