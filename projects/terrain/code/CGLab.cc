//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "CGlab.h"
#include <chrono>
#include "foundation/math/matrix4D.h"
#include <assert.h>
#include "render/resources/HalfEdgeMesh.h"
#include "terrain.h"
#include "imguiextra.h"
#include "UserInterface.h"
#include "foundation/util/JsonParser.h"

using namespace CGMath;

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
CGLab::CGLab()
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
		render = std::make_shared<Render::Render>();
		shaders = std::make_shared<CGMath::ShaderObject>();
		input = std::make_shared<Input::InputManager>(this->window, shaders.get(), &lNode);

		render->Init(this->GetWindow()->GetWidth(), this->GetWindow()->GetHeight());

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

		ImGui::SetupImGuiStyle(true, 1.0f);

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

	std::shared_ptr<CGMath::MeshResources> mesh = std::make_shared<CGMath::MeshResources>();
	//std::shared_ptr<CGMath::TextureResource> tex = std::make_shared<CGMath::TextureResource>();
	std::shared_ptr<TerrainEditor::Terrain> terrain = std::make_shared<TerrainEditor::Terrain>();
		
	vec = vector4D(0, 0, 0);

	matrix4D modelMat = matrix4D::translationMatrix(vector4D(0.0f, 0.0f, 0.0f));
		
	matrix4D perspective = matrix4D::perspectiveMatrix();

	terrain->CreateTerrain("resources/textures/perlin2.jpg", 1.0f, 50.0f);
	terrain->GenerateBuffer();

	//mesh->createQuad();
	//mesh->loadObj("resources/models/ARC170.obj");

	//mesh->genBuffer();
		
	shaders->setupShaders("resources/shaders/Blinn-phong.vert", "resources/shaders/Blinn-phong.frag");

	lNode.setShaders(shaders);

	lNode.setPos(0.0f, 250.0f, 0.0f);
	lNode.setIntensity(0.6f, 0.6f, 0.6f);
	lNode.setColour(0.2f, 0.2f, 0.2f);
    lNode.apply();
    shaders->setupVector3f("cameraPosition", 0.0f, 0.0f, 0.0f);
    shaders->setupVector3f("u_matAmbientReflectance", 1.0f, 1.0f, 1.0f);
    shaders->setupVector3f("u_matDiffuseReflectance", 1.0f, 1.0f, 1.0f);
    shaders->setupVector3f("u_matSpecularReflectance", 1.0f, 1.0f, 1.0f);
    shaders->setupUniformFloat("u_matShininess", 64.0f);
	shaders->setupMatrix4fv("transMatrix", modelMat);

	textures.AddTexture("resources/textures/water.jpg");
	textures.AddTexture("resources/textures/sand.jpg");
	textures.AddTexture("resources/textures/grass.jpg");
	textures.AddTexture("resources/textures/rock.jpg");

	textures.AddTexture("resources/textures/pathway.jpg");
	textures.AddTexture("resources/textures/road.jpg");
	//tex->LoadNormalFile("resources/textures/nm_heightmap.png");

	shaders->setupUniformInt("textures[0]", 0);
	shaders->setupUniformInt("textures[1]", 1);
	shaders->setupUniformInt("textures[2]", 2);
	shaders->setupUniformInt("textures[3]", 3);
	shaders->setupUniformInt("pathtex", 4);
	shaders->setupUniformInt("textures[4]", 5);

	//gN.setMesh(mesh);
	gN.setTerrain(terrain);
	gN.setShaders(shaders);
	gN.setTex(std::make_shared<CGMath::TextureNode>(textures));

    std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();

	while (this->window->IsOpen())
	{
		input->Update();

		/*render->BindForWriting();*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
		this->window->Update();

		matrix4D transMat;

		float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);


		mouseX = (input->GetOldX()*2.0f) / 1024 - 1;
		mouseY = 1 - (input->GetOldY()*2.0f) / 768;

		matrix4D rotx = matrix4D::rotX(-input->GetRotY()*0.001f);
		matrix4D roty = matrix4D::rotY(input->GetRotX()*0.001f);

		matrix4D rotation = (roty*rotx);

		const vector4D left = rotation.GetXAxis();
		const vector4D up = rotation.GetYAxis();
		const vector4D forward = rotation.GetZAxis();

		vec = vec + (rotation * input->GetMovement());

		gN.SetProjectionMat(perspective);
		gN.SetViewMat(matrix4D::LookAt(vec, vec + forward, up));

		vector3D camPos(gN.GetViewMat()[3], gN.GetViewMat()[7], gN.GetViewMat()[11]);

		shaders->setupVector3f("cameraPosition", camPos);

		gN.setTransMat(modelMat);
		gN.draw();

        glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->window->SwapBuffers();
	}
}


	//------------------------------------------------------------------------------

} // namespace Example