#include "config.h"
#include "frameserver.h"
#include "render/frames/lightcullingpass.h"
#include "render/render/renderer.h"

namespace Render
{
FrameServer::FrameServer()
{
	
}

void FrameServer::SetupFramePasses()
{
	// depth pre-pass
	this->Depth = DepthPass::Create();
	this->Depth->name = "Depth";
	this->Depth->Setup();

	this->framePassByName.Add(this->Depth->name, this->Depth.upcast<Render::FramePass>());
	this->framePasses.Append(this->Depth.downcast<Render::FramePass>());

	// IBL pass
	this->IBL = IBLPass::Create();
	this->IBL->name = "IBL";
	this->IBL->Setup();

	this->framePassByName.Add(this->IBL->name, this->IBL.upcast<Render::FramePass>());
	this->framePasses.Append(this->IBL.downcast<Render::FramePass>());

	//Light culling compute shader pass
	this->lightCullingPass = LightCullingPass::Create();
	this->lightCullingPass->name = "LightCulling";
	this->lightCullingPass->Setup();
	
	this->framePassByName.Add(this->lightCullingPass->name, this->lightCullingPass);
	this->framePasses.Append(this->lightCullingPass);

	//SSAO shader pass
	this->ssaoPass = SSAOPass::Create();
	this->ssaoPass->name = "SSAO";
	this->ssaoPass->Setup();
	this->framePassByName.Add(this->ssaoPass->name, this->ssaoPass.downcast<Render::FramePass>());
	this->framePasses.Append(this->ssaoPass.downcast<FramePass>());

	// FlatGeometryLit pass
	this->FlatGeometryLit = FlatGeometryLitPass::Create();
	this->FlatGeometryLit->name = "FlatGeometryLit";
	this->FlatGeometryLit->Setup();
	
	this->framePassByName.Add(this->FlatGeometryLit->name, this->FlatGeometryLit.downcast<Render::FramePass>());
	this->framePasses.Append(this->FlatGeometryLit.downcast<FramePass>());

	// PostProcessing pass
	this->postProcessing = PostProcessingPass::Create();
	this->postProcessing->name = "PostProcessing";
	this->postProcessing->Setup();

	this->framePassByName.Add(this->postProcessing->name, this->postProcessing.downcast<Render::FramePass>());
	this->framePasses.Append(this->postProcessing.downcast<FramePass>());

	Renderer::Instance()->SetFinalColorBuffer(this->postProcessing->buffer);
	Renderer::Instance()->SetRenderFBO(this->postProcessing->frameBufferObject);
}

void FrameServer::UpdateResolutions() const
{
	this->Depth->UpdateResolution();
	this->ssaoPass->UpdateResolution();
	this->FlatGeometryLit->UpdateResolution();
	this->postProcessing->UpdateResolution();
}

Ptr<FramePass> FrameServer::GetFramePass(const Util::String& name)
{
	if (this->HasPassNamed(name))
		return this->framePassByName[name];
	else
	{
		printf("ERROR: No framepass named %s!\n", name.AsCharPtr());
		assert(false);
		return nullptr;
	}
}

bool FrameServer::HasPassNamed(const Util::String& name)
{
	return this->framePassByName.Contains(name);
}

Ptr<DepthPass> FrameServer::GetDepthPass()
{
	return this->Depth;
}

Ptr<IBLPass> FrameServer::GetIBLPass()
{
	return this->IBL;
}

Ptr<FramePass> FrameServer::GetLightCullingPass()
{
	return this->lightCullingPass;
}

Ptr<FlatGeometryLitPass> FrameServer::GetFlatGeometryLitPass()
{
	return this->FlatGeometryLit;
}
Ptr<SSAOPass> FrameServer::GetSSAOPass()
{
	return this->ssaoPass;
}

Ptr<PostProcessingPass> FrameServer::GetPostProcessingPass()
{
	return this->postProcessing;
}
}
