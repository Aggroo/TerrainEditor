#include "config.h"
#include "depthpass.h"
#include "render/render/renderer.h"
#include "render/server/shaderserver.h"
#include "render/resources/material.h"
#include "render/resources/model.h"

namespace Render
{
__ImplementClass(Render::DepthPass, 'REDE', Render::DrawPass)
DepthPass::DepthPass()
{
}

DepthPass::~DepthPass()
{

}

void DepthPass::Setup()
{
	//Generate framebuffer
	glGenFramebuffers(1, &this->frameBufferObject);

	//Texture border color
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const Resolution& res = Renderer::Instance()->GetRenderResolution();

	//Generate framebuffer render buffer
	glGenRenderbuffers(1, &this->buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, res.x, res.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Generate position buffer texture
	glGenTextures(1, &this->gPosition);
	glBindTexture(GL_TEXTURE_2D, this->gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, res.x, res.y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// - normal color buffer
	glGenTextures(1, &this->gNormal);
	glBindTexture(GL_TEXTURE_2D, this->gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, res.x, res.y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// - color + specular color buffer
	glGenTextures(1, &this->gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, this->gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.x, res.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	//Bind linear depth buffer to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gPosition, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->gNormal, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->gAlbedoSpec, 0);

	const GLenum drawBuffers[3]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, &drawBuffers[0]);

	_assert2(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Depth Framebuffer Status Error!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FramePass::Setup();
}

void DepthPass::Execute()
{
	this->BindFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint currentProgram = 0;

	for (Material* material : this->materials)
	{
		auto shader = material->GetShader(this->name);

		if (shader->GetProgram() != currentProgram)
		{
			currentProgram = shader->GetProgram();
			glUseProgram(currentProgram);
		}

		shader->EnableRenderState();

		for (auto surface : material->SurfaceList())
		{
			//surface->GetTextureList()->BindTextures();

			
			for (auto modelNode : surface->GetModelNodes())
			{
				//Bind mesh
				//TODO: We should probably check and make sure we don't bind these more than once
				//modelNode->Draw();
				modelNode->modelInstance->GetMesh()->Bind();

				shader->setupMatrix4fv("Model", Math::mat4::identity());
				modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);

				modelNode->modelInstance->GetMesh()->Unbind();
			}
		}
	}

	//Unbind Depth FrameBufferObject
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FramePass::Execute();
}

void DepthPass::UpdateResolution()
{
	const Resolution& newRes = Renderer::Instance()->GetRenderResolution();

	//Rebinds the framebuffer render buffer to the new resolution
	glBindRenderbuffer(GL_RENDERBUFFER, this->buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newRes.x, newRes.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Rebinds the depth buffer texture to the new resolution
	glBindTexture(GL_TEXTURE_2D, this->gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, newRes.x, newRes.y, 0, GL_RGB, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, this->gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, newRes.x, newRes.y, 0, GL_RGB, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, this->gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newRes.x, newRes.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}
}
