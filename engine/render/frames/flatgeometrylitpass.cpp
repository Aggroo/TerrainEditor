#include "config.h"
#include "flatgeometrylitpass.h"
#include "lightcullingpass.h"
#include "render/render/renderer.h"
#include "render/server/frameserver.h"

namespace Render
{
__ImplementClass(Render::FlatGeometryLitPass, 'REFP', Render::DrawPass)
FlatGeometryLitPass::FlatGeometryLitPass()
{

}

FlatGeometryLitPass::~FlatGeometryLitPass()
{
	
}

void FlatGeometryLitPass::Setup()
{
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const Resolution& res = Renderer::Instance()->GetRenderResolution();

	//Generate the frambuffer render texture
	glGenTextures(1, &this->buffer);
	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, res.x, res.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Generate color framebuffer object
	glGenFramebuffers(1, &this->frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->buffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FrameServer::Instance()->GetDepthPass()->GetTextureBuffer());

	const GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawBuffers[0]);

	_assert2(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FlatGeometryLit Framebuffer Status Error!");
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FramePass::Setup();
}

void FlatGeometryLitPass::Execute()
{
	this->BindFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawPass::Execute();

	FramePass::Execute();
}

void FlatGeometryLitPass::UpdateResolution()
{
	const Resolution& newRes = Renderer::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}
}
