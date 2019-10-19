#include "config.h"
#include "renderer.h"
#include "render/window.h"
#include "render/camera/camera.h"
#include "render/server/frameserver.h"
#include "render/server/lightserver.h"
#include "render/server/shaderserver.h"

namespace Render
{
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Setup(Display::Window* window)
{
	this->window = window;
	this->renderResolution = { window->GetWidth(), window->GetHeight() };
	glGenBuffers(1, this->ubo);

	//Setup framepasses before materials
	Render::FrameServer::Instance()->SetupFramePasses();

}

void Renderer::Render(bool drawToScreen)
{
	//Set resolution of the viewport for the render texture
	glViewport(0, 0, renderResolution.x, renderResolution.y);
	SetupUniformBuffer(Graphics::MainCamera::Instance());

	for (auto pass : FrameServer::Instance()->framePasses)
	{
		pass->Execute();
	}

	glViewport(0, 0, windowResolution.x, windowResolution.y);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK); // Set the back buffer as the draw buffer

	//Copy final colorbuffer to screen if specified
	if (drawToScreen)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->renderFBO);
		glBlitFramebuffer(0, 0, this->renderResolution.x, this->renderResolution.y, 0, 0, this->windowResolution.x, this->windowResolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		//This is only for OGL 4.5 and it might cause issues with older cards...
		//glBlitNamedFramebuffer(FrameServer::Instance()->FlatGeometryLit->frameBufferObject, 0, 0, 0, this->renderResolution.x, this->renderResolution.y, 0, 0, this->windowResolution.x, this->windowResolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	Render::ShaderServer::Instance()->Update();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);

}

const Resolution& Renderer::GetRenderResolution() const
{
	return this->renderResolution;
}

void Renderer::SetRenderResolution(const Resolution& res)
{
	this->renderResolution = res;
	Graphics::MainCamera::Instance()->UpdateProjectionMatrix();
	FrameServer::Instance()->UpdateResolutions();
	LightServer::Instance()->UpdateWorkGroups();
	LightServer::Instance()->UpdatePointLightBuffer();
	LightServer::Instance()->UpdateSpotLightBuffer();
}

void Renderer::SetRenderResolution(const int& x, const int& y)
{
	SetRenderResolution({ x, y });
}

void Renderer::SetWindowResolution(const int& x, const int& y)
{
	this->windowResolution = { x, y };
}

void Renderer::SetupUniformBuffer(Graphics::Camera* camera)
{
	uniformBufferBlock.View = !camera->GetView();
	uniformBufferBlock.InvView = !camera->GetInvView();

	uniformBufferBlock.Projection = !camera->GetProjection();
	uniformBufferBlock.InvProjection = !camera->GetInvProjection();

	uniformBufferBlock.ViewProjection = !camera->GetViewProjection();
	uniformBufferBlock.InvViewProjection = !camera->GetInvViewProjection();

	uniformBufferBlock.viewToTextureSpace = camera->GetViewToTextureSpaceMatrix();

	uniformBufferBlock.CameraPosition = camera->GetPosition();

	uniformBufferBlock.ScreenSize = this->renderResolution;

	uniformBufferBlock.TimeAndRandom[0] = (GLfloat)glfwGetTime();
	uniformBufferBlock.TimeAndRandom[1] = 0.f;

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformBufferBlock), &uniformBufferBlock, GL_STATIC_DRAW);
}
}
