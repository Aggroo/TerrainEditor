#include "config.h"
#include "drawpass.h"
#include "application/basegamefeatures/entitymanager.h"


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

void DrawPass::Execute()
{
	//if (!skybox.isvalid())
	//{
	//	skybox = Render::Skybox::Create();
	//	skybox->Activate();
	//}
	BaseGameFeature::EntityManager::Instance()->Update();
	//skybox->Update();

	FramePass::Execute();
}

void DrawPass::UpdateResolution()
{

}
}
