#include "config.h"
#include "drawpass.h"
#include "render/resources/material.h"
#include "render/resources/model.h"
#include "render/server/frameserver.h"

namespace Render
{
__ImplementClass(Render::DrawPass, 'REDP', Render::FramePass)
DrawPass::DrawPass()
{
	this->frameBufferObject = 0;
	this->buffer = 0;
}

DrawPass::~DrawPass()
{
	
}

void DrawPass::BindFrameBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frameBufferObject);
}

void DrawPass::Setup()
{
	FramePass::Setup();
}

void DrawPass::AddMaterial(Ptr<Render::Material> mat) 
{ 
	this->materials.Append(mat); 
}

void DrawPass::Execute()
{
	GLuint currentProgram = 0;

	for (Render::Material* material : this->materials)
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
			surface->GetTextureList()->BindTextures();
			surface->BindUniformBuffer();

			for (auto modelNode : surface->GetModelNodes())
			{
				//Bind mesh
				//TODO: We should probably check and make sure we don't bind these more than once
				//modelNode->Draw();
				modelNode->modelInstance->GetMesh()->Bind();

				shader->setupMatrix4fv("Model", modelNode->modelInstance->transform);
				modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);

				modelNode->modelInstance->GetMesh()->Unbind();
			}
			surface->GetTextureList()->UnbindTextures();
		}
	}

	FramePass::Execute();
}

void DrawPass::UpdateResolution()
{

}
}
