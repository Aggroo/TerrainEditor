#include "config.h"
#include "ssaopass.h"
#include "render/render/renderer.h"
#include "render/server/frameserver.h"
#include "render/server/shaderserver.h"
#include "foundation/math/math.h"
#include <random>

namespace Render
{
__ImplementClass(Render::SSAOPass, 'SSAO', Render::DrawPass)
SSAOPass::SSAOPass()
{
	this->ssaoShader = ShaderObject::Create();
	this->ssaoBlurShader = ShaderObject::Create();
	this->ssaoQuad = MeshResources::Create();
	this->ssaoColorBufferBlur = TextureResource::Create();
	this->noiseTexture = TextureResource::Create();
	this->ssaoColorBuffer = TextureResource::Create();
	this->defaultAO = TextureResource::Create();
}

SSAOPass::~SSAOPass()
{

}

void SSAOPass::Setup()
{

	//Texture border color
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const Resolution& res = Renderer::Instance()->GetRenderResolution();

	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;

	for (unsigned int i = 0; i < 64; ++i)
	{
		Math::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = Math::vec3::Normalize(sample);
		sample = sample * randomFloats(generator);
		float scale = (float)i / 64.0;
		scale = Math::Lerp(0.1f, 1.0f, scale * scale);
		sample = sample * scale;

		this->ssaoKernel.Append(sample);
	}

	for (unsigned int i = 0; i < 16; i++)
	{
		Math::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		this->ssaoNoise.Append(noise);
	}

	//Generate noise texture
	glGenTextures(1, &this->noiseTexture->GetTextureID());
	glBindTexture(GL_TEXTURE_2D, this->noiseTexture->GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, &this->ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// - ssao color buffer
	glGenTextures(1, &this->ssaoColorBuffer->GetTextureID());
	glBindTexture(GL_TEXTURE_2D, this->ssaoColorBuffer->GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, res.x, res.y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	//Generate ssao framebuffer
	glGenFramebuffers(1, &this->ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->ssaoFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->ssaoColorBuffer->GetTextureID(), 0);

	_assert2(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Framebuffer Status Error!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &this->ssaoColorBufferBlur->GetTextureID());
	glBindTexture(GL_TEXTURE_2D, this->ssaoColorBufferBlur->GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, res.x, res.y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Generate blur framebuffer
	glGenFramebuffers(1, &this->ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->ssaoBlurFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->ssaoColorBufferBlur->GetTextureID(), 0);

	_assert2(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Blur Framebuffer Status Error!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	this->ssaoShader = Render::ShaderServer::Instance()->GetShader("ssao");

	this->ssaoShader->BindProgram();
	this->ssaoShader->setupUniformInt("gPosition", 0);
	this->ssaoShader->setupUniformInt("gNormal", 1);
	this->ssaoShader->setupUniformInt("texNoise", 2);

	this->ssaoBlurShader = Render::ShaderServer::Instance()->GetShader("ssaoblur");

	this->ssaoBlurShader->BindProgram();
	this->ssaoShader->setupUniformInt("ssaoInput", 0);

	this->ssaoQuad->createQuad();
	this->ssaoQuad->GenerateBuffers();

	this->defaultAO->LoadTextureFile("resources/textures/terrain_textures/default/defaultAO.png");

	FramePass::Setup();
}

void SSAOPass::Execute()
{
	if(Render::Renderer::Instance()->IsRenderFlagSet(Render::RendererSettings::RenderSSAO))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->ssaoFBO);
		glClear(GL_COLOR_BUFFER_BIT);

		this->ssaoShader->BindProgram();

		// Send kernel + rotation 
		for (unsigned int i = 0; i < 64; ++i)
		{
			Util::String name = "samples[";
			name.AppendInt(i); name.Append("]");
			this->ssaoShader->setupVector3f(name.AsCharPtr(), ssaoKernel[i]);
		}
			
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetTextureBuffer());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetNormalBuffer());
		this->noiseTexture->bindTex(2);

		this->ssaoQuad->Draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glBindFramebuffer(GL_FRAMEBUFFER, this->ssaoBlurFBO);
		glClear(GL_COLOR_BUFFER_BIT);

		this->ssaoBlurShader->BindProgram();
		this->ssaoColorBuffer->bindTex(0);
		this->ssaoQuad->Draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		FramePass::Execute();
	}
}


void SSAOPass::UpdateResolution()
{
	const Resolution& newRes = Renderer::Instance()->GetRenderResolution();

	//Rebinds the depth buffer texture to the new resolution
	glBindTexture(GL_TEXTURE_2D, this->ssaoColorBuffer->GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, newRes.x, newRes.y, 0, GL_RGB, GL_FLOAT, NULL);
	
	glBindTexture(GL_TEXTURE_2D, this->ssaoColorBufferBlur->GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, newRes.x, newRes.y, 0, GL_RGB, GL_FLOAT, NULL);


	glBindTexture(GL_TEXTURE_2D, 0);
}

Ptr<TextureResource> SSAOPass::GetSSAOBuffer()
{
	if (Render::Renderer::Instance()->IsRenderFlagSet(Render::RendererSettings::RenderSSAO))
	{
		return this->ssaoColorBufferBlur;
	}
	else
	{
		return this->defaultAO;
	}
}
}
