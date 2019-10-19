#include "config.h"
#include "postprocessingpass.h"
#include "lightcullingpass.h"
#include "render/render/renderer.h"
#include "render/server/frameserver.h"
#include "render/server/shaderserver.h"

namespace Render
{
__ImplementClass(Render::PostProcessingPass, 'REPP', Render::DrawPass)
PostProcessingPass::PostProcessingPass()
{
	this->postProcessingShader = ShaderObject::Create();
	this->postProcessingQuad = MeshResources::Create();
}

PostProcessingPass::~PostProcessingPass()
{
	
}

void PostProcessingPass::Setup()
{
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const Resolution& res = Renderer::Instance()->GetRenderResolution();

	//Generate the frambuffer render texture
	glGenTextures(1, &this->buffer);
	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, res.x, res.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);


	//Generate hdr framebuffer object
	glGenFramebuffers(1, &this->frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->buffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FrameServer::Instance()->GetDepthPass()->GetTextureBuffer());

	const GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &drawBuffers[0]);

	_assert2(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "HDR Framebuffer Status Error!");
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint vert = Render::ShaderServer::Instance()->LoadVertexShader("resources/shaders/postprocessing.vert");
	GLuint frag = Render::ShaderServer::Instance()->LoadFragmentShader("resources/shaders/postprocessing.frag");

	this->postProcessingShader->AddShader(vert);
	this->postProcessingShader->AddShader(frag);
	this->postProcessingShader->LinkShaders();
	
	Render::ShaderServer::Instance()->AddShaderObject("Post-Processing", this->postProcessingShader);

	this->postProcessingShader->BindProgram();

	this->postProcessingShader->setupUniformInt("hdrBuffer", 0);

	this->postProcessingQuad->createQuad();
	this->postProcessingQuad->genBuffer();

	FramePass::Setup();
}

void PostProcessingPass::Execute()
{
	this->BindFrameBuffer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Has to be handled better...
	this->postProcessingShader->BindProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetFlatGeometryLitPass()->GetTextureBuffer());

	this->postProcessingQuad->drawMesh();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FramePass::Execute();
}

void PostProcessingPass::UpdateResolution()
{
	const Resolution& newRes = Renderer::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}
}
