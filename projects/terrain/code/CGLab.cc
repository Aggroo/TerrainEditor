//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "CGlab.h"
#include <chrono>
#include "foundation/math/matrix4D.h"
#include <assert.h>
#include <algorithm>
#include "render/resources/HalfEdgeMesh.h"
#include "terrain.h"

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
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
		if (mRightCheck == true)
		{
			if (key == GLFW_KEY_W && action == GLFW_PRESS)
			{
				wFlag = true;
			}
			else if (key == GLFW_KEY_S && action == GLFW_PRESS)
			{
				sFlag = true;
			}
			else if (key == GLFW_KEY_D && action == GLFW_PRESS)
			{
				dFlag = true;
			}
			else if (key == GLFW_KEY_A && action == GLFW_PRESS)
			{
				aFlag = true;
			}
			else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
			{
				shiftFlag = true;
			}

		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			wFlag = false;
		}
		else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			sFlag = false;
		}
		else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			dFlag = false;
		}
		else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			aFlag = false;
		}
		else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		{
			shiftFlag = false;
		}
		else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			this->window->Close();
		}
		else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		{
			lNode.setIntensity(0.8f, 0.8f, 0.8f);
			lNode.setPos(50.0f, 80.0f, -80.0f);
			lNode.apply();
		}
		else if (key == GLFW_KEY_U && action == GLFW_PRESS)
		{
			lNode.setIntensity(1.0f, 0.4f, 0.4f);
			lNode.apply();
		}
		else if (key == GLFW_KEY_I && action == GLFW_PRESS)
		{
			lNode.setIntensity(0.4f, 1.0f, 0.4f);
			lNode.apply();
		}
		else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		{
			lNode.setIntensity(0.5f, 0.5f, 1.0f);
			lNode.apply();
		}
		else if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		{
			shaders->ReloadShaders();
			shaders->setupVector3f("u_matAmbientReflectance", 1.0f, 1.0f, 1.0f);
			shaders->setupVector3f("u_matDiffuseReflectance", 1.0f, 1.0f, 1.0f);
			shaders->setupVector3f("u_matSpecularReflectance", 1.0f, 1.0f, 1.0f);
			shaders->setupUniformFloat("u_matShininess", 64.0f);

			shaders->setupUniformInt("textures[0]", 0);
			shaders->setupUniformInt("textures[1]", 1);
			shaders->setupUniformInt("textures[2]", 2);
			shaders->setupUniformInt("textures[3]", 3);
			shaders->setupUniformInt("pathtex", 4);
			shaders->setupUniformInt("textures[4]", 5);

			lNode.apply();
		}
		

	});
	
	window->SetMouseMoveFunction([this](float64 x, float64 y)
	{
		if (mRightCheck)
		{
			rotX += x - oldX;
			rotY += y - oldY;
		}

		oldX = x;
		oldY = y;

	});

	window->SetMousePressFunction([this](int32 button, int32 action, int32 mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mLeftCheck = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			mLeftCheck = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			mRightCheck = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			mRightCheck = false;
		}	
	});
	
	window->SetMouseScrollFunction([this](float64 x, float64 y)
	{
		if (mLeftCheck == true)
		{
			scrollY += y;
		}
		
	});

	if (this->window->Open())
	{
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
		
		return true;
	}
	return false;
}

/**
*/
	void
	CGLab::Run()
	{

		std::shared_ptr<CGMath::MeshResources> mesh = std::make_shared<CGMath::MeshResources>();
		//std::shared_ptr<CGMath::TextureResource> tex = std::make_shared<CGMath::TextureResource>();
		shaders = std::make_shared<CGMath::ShaderObject>();
		std::shared_ptr<TerrainEditor::Terrain> terrain = std::make_shared<TerrainEditor::Terrain>();
		
		vec = vector4D(0, 0, 0);

		matrix4D modelMat = matrix4D::translationMatrix(vector4D(0.0f, 0.0f, 0.0f));
		
		matrix4D perspective = matrix4D::perspectiveMatrix();

		terrain->CreateTerrain("resources/textures/heightmap2.jpg", 1.0f, 50.0f);
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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
			this->window->Update();

			vector4D movement;

			matrix4D transMat;

			float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);

			if (wFlag == true)
			{
				if (shiftFlag)
				{
					movement[2] += movSpeed*movMultiplier;
				}
				else
				{
					movement[2] += movSpeed;
				}
				
			}
			if (sFlag == true)
			{
				if (shiftFlag)
				{
					movement[2] -= movSpeed*movMultiplier;
				}
				else
				{
					movement[2] -= movSpeed;
				}
				
			}
			if (dFlag == true)
			{
				if (shiftFlag)
				{
					movement[0] -= movSpeed*movMultiplier;
				}
				else
				{
					movement[0] -= movSpeed;
				}
				
			}
			if (aFlag == true)
			{
				if (shiftFlag)
				{
					movement[0] += movSpeed*movMultiplier;
				}
				else
				{
					movement[0] += movSpeed;
				}
				
			}

			mouseX = (oldX*2.0f) / 1024 - 1;
			mouseY = 1 - (oldY*2.0f) / 768;

			matrix4D rotx = matrix4D::rotX(-rotY*0.001f);
			matrix4D roty = matrix4D::rotY(rotX*0.001f);

			matrix4D rotation = (roty*rotx);

			const vector4D left = rotation.GetXAxis();
			const vector4D up = rotation.GetYAxis();
			const vector4D forward = rotation.GetZAxis();

			vec = vec + (rotation * movement);

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