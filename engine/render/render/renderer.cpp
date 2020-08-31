#include "config.h"
#include "renderer.h"
#include "render/window.h"
#include "render/camera/camera.h"
#include "render/server/frameserver.h"
#include "render/server/lightserver.h"
#include "render/server/shaderserver.h"
#include "render/server/resourceserver.h"
#include "foundation/math/math.h"

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
	T_CORE_TRACE("RENDERER Setup Render Resolution");

	this->window = window;
	this->renderResolution = { window->GetWidth(), window->GetHeight() };
	glGenBuffers(1, this->ubo);

	T_CORE_TRACE("RENDERER Setup Render Flags");
	renderFlags |= RenderSSAO;

	T_CORE_TRACE("RENDERER Setup Render Options");
	renderOptions.Add(OptionTonemapping, static_cast<float>(ToneMappingACES));
	renderOptions.Add(OptionExposure, 0.0f);
	renderOptions.Add(OptionGamma, 2.2f);
	renderOptions.Add(OptionAOKernel, 64.0f);
	renderOptions.Add(OptionAORadius, 0.5f);
	renderOptions.Add(OptionAOBias, 0.025f);

	T_CORE_TRACE("RENDERER Setup Shaders");
	Render::ShaderServer::Instance()->SetupShaders("resources/shaders/shaders.json");

	T_CORE_TRACE("RENDERER Setup Frame passes");
	//Setup framepasses before materials
	Render::FrameServer::Instance()->SetupFramePasses();

	T_CORE_TRACE("RENDERER Setup Materials");
	Render::ResourceServer::Instance()->SetupMaterials("resources/materials.json");

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
	T_CORE_TRACE("RENDERER Set Render Resolutions");
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
	uniformBufferBlock.TimeAndRandom[1] = Math::Hash(glfwGetTime());

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformBufferBlock), &uniformBufferBlock, GL_STATIC_DRAW);
}
}
