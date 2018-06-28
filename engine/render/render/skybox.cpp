#include "config.h"
#include "skybox.h"
#include <iostream>
#include "render/camera/camera.h"
#include "renderer.h"
#include "render/server/shaderserver.h"

namespace Render
{
__ImplementClass(Render::Skybox, 'RSKY', Game::Entity)
Skybox::Skybox()
{
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

	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg2/right.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg2/left.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg2/top.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg2/bottom.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg2/front.bmp");
	cubeTextures.Append("resources/textures/skyboxes/skyboxsun5deg2/back.bmp");
	LoadCubemap(cubeTextures);
	GenerateCube();

	Game::Entity::Activate();
}

void Skybox::Deactivate()
{
	Game::Entity::Deactivate();
}

void Skybox::Update()
{
	glDepthFunc(GL_LEQUAL);
	shader->BindProgram();
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

void Skybox::LoadCubemap(Util::Array<Util::String> texures)
{
	glGenTextures(1, &cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
	
	int width, height, nrChannels;
	for (unsigned int i = 0; i < texures.Size(); i++)
	{
		unsigned char *data = stbi_load(texures[i].AsCharPtr(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			printf("Cubemap texture failed to load at path: %s", texures[i].AsCharPtr());
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
