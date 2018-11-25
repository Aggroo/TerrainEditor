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

	// depth pre-pass
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

	// FlatGeometryLit pass
	this->FlatGeometryLit = FlatGeometryLitPass::Create();
	this->FlatGeometryLit->name = "FlatGeometryLit";
	this->FlatGeometryLit->Setup();
	
	this->framePassByName.Add(this->FlatGeometryLit->name, this->FlatGeometryLit.downcast<Render::FramePass>());
	this->framePasses.Append(this->FlatGeometryLit.downcast<FramePass>());

	Renderer::Instance()->SetFinalColorBuffer(this->FlatGeometryLit->buffer);
}

void FrameServer::UpdateResolutions() const
{
	this->Depth->UpdateResolution();
	this->FlatGeometryLit->UpdateResolution();
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
}
