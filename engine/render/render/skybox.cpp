#include "config.h"
#include "skybox.h"
#include <iostream>
#include "render/camera/camera.h"
#include "renderer.h"
#include "render/server/shaderserver.h"

namespace Render
{
__ImplementClass(Render::Skybox, 'RSKY', Game::EntityBase)
Skybox::Skybox()
{
	cubemap = TextureResource::Create();
	shader = Render::ShaderObject::Create();
}

Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Skybox::Activate()
{
	GLuint vert = Render::ShaderServer::Instance()->LoadVertexShader("resources/shaders/skybox.vert");
	GLuint frag = Render::ShaderServer::Instance()->LoadFragmentShader("resources/shaders/skybox.frag");

	shader->AddShader(vert);
	shader->AddShader(frag);
	shader->LinkShaders();

	Render::ShaderServer::Instance()->AddShaderObject("Skybox", shader);

	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg/right.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg/left.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg/top.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg/bottom.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg/front.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg/back.bmp");

	cubemap->LoadCubemap(cubeTextures);

	GenerateCube();

	Game::EntityBase::Activate();
}

void Skybox::Deactivate()
{
	Game::EntityBase::Deactivate();
}

void Skybox::Update()
{
	glDepthFunc(GL_LEQUAL);
	shader->BindProgram();
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->GetTextureID());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::GenerateCube()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	for (int i = 0; i < 108; ++i)
	{
		cubeVerts.Append(skyboxVertices[i]);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cubeVerts.Size(), &cubeVerts[0], GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
}
