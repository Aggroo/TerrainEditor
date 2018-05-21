#include "config.h"
#include "renderer.h"
#include "render/window.h"
#include "render/camera/camera.h"

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
}

const Resolution& Renderer::GetRenderResolution() const
{
	return this->renderResolution;
}

void Renderer::SetRenderResolution(const Resolution& res)
{
	this->renderResolution = res;
}

void Renderer::SetRenderResolution(const int& x, const int& y)
{
	SetRenderResolution({ x, y });
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

	uniformBufferBlock.CameraPosition = camera->GetCameraPosition();

	uniformBufferBlock.ScreenSize = this->renderResolution;

	uniformBufferBlock.TimeAndRandom[0] = (GLfloat)glfwGetTime();
	uniformBufferBlock.TimeAndRandom[1] = 0.f;

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformBufferBlock), &uniformBufferBlock, GL_STATIC_DRAW);
}
}
