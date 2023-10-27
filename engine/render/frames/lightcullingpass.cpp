#include "config.h"
#include "lightcullingpass.h"
#include "render/server/shaderserver.h"
#include "render/resources/shaderobject.h"
#include "render/server/frameserver.h"

namespace Render
{
__ImplementClass(Render::LightCullingPass, 'RELC', Render::FramePass)
LightCullingPass::LightCullingPass()
{
	lightServer = LightServer::Instance();

	glGenBuffers(1, this->ubo);
}

LightCullingPass::~LightCullingPass()
{
	
}

void LightCullingPass::Setup()
{
	T_CORE_TRACE("LIGHTCULLING PASS Generating pass");
	this->lightCullingProgram = ShaderServer::Instance()->GetShader("lightculling")->GetProgram();

	FramePass::Setup();
}

void LightCullingPass::Execute()
{
	glUseProgram(this->lightCullingProgram);

	uniformBlock.pointLightCount = (GLint)LightServer::Instance()->GetNumPointLights();
	uniformBlock.spotLightCount = (GLint)LightServer::Instance()->GetNumSpotLights();

	//Bind uniform buffer block
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 24, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBlock), &uniformBlock, GL_STATIC_DRAW);

	//Bind depth texture to the light shader
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(this->lightCullingProgram, "depthMap"), 4);
	glBindTexture(GL_TEXTURE_2D, FrameServer::Instance()->GetDepthPass()->GetPositionBuffer());

	//
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightServer->GetPointLightBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, lightServer->GetSpotLightBuffer());

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, lightServer->GetVisiblePointLightIndexBuffer());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, lightServer->GetVisibleSpotLightIndexBuffer());

	glDispatchCompute(lightServer->GetWorkGroupsX(), lightServer->GetWorkGroupsY(), 1);

	// Unbind the depth map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);

	FramePass::Execute();
}

}
