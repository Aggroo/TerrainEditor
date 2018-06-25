#include "config.h"
#include "depthpass.h"
#include "render/render/renderer.h"
#include "application/basegamefeatures/entitymanager.h"

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

	//Generate linear depth buffer texture
	glGenTextures(1, &this->linearDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, this->linearDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, res.x, res.y, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Bind linear depth buffer to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->linearDepthBuffer, 0);

	const GLenum drawBuffers[1]{ GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawBuffers[0]);

	_assert2(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Depth Framebuffer Status Error!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FramePass::Setup();
}

void DepthPass::Execute()
{
	this->BindFrameBuffer();
	glClear(GL_DEPTH_BUFFER_BIT);

	BaseGameFeature::EntityManager::Instance()->Update();

	//Unbind Depth FrameBufferObject
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	glBindTexture(GL_TEXTURE_2D, this->linearDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, newRes.x, newRes.y, 0, GL_RED, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}
}
