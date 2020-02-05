#pragma once
#include "core/app.h"
#include "foundation/math/mat4.h"
#include "core/singleton.h"
#include "foundation/util/dictionary.h"

namespace Display { class Window; }
namespace Graphics { class Camera; }

namespace Render
{
struct Resolution
{
	GLint x;
	GLint y;
};

enum RendererSettings : uint32_t
{
	RenderHDR = 1 << 0,
	RenderBloom = 1 << 1,
	RenderSSAO = 1 << 2
};

enum RendererToneMappingTypes
{
	ToneMappingOff,
	ToneMappingACES,
	ToneMappingUncharted2,
	ToneMappingReinhard
};

enum RendererOptions
{
	OptionTonemapping,
	OptionExposure,
	OptionGamma
};

class Renderer
{
__DeclareSingleton(Renderer)
private:
	struct UniformBufferBlock
	{
		Math::mat4 View;
		Math::mat4 Projection;
		Math::mat4 ViewProjection;
		Math::mat4 InvView;
		Math::mat4 InvProjection;
		Math::mat4 InvViewProjection;
		Math::mat4 viewToTextureSpace;
		Math::vec4 CameraPosition;
		Resolution ScreenSize;
		GLfloat TimeAndRandom[2];
		GLuint LightTileWorkGroups[2];

	} uniformBufferBlock;

	GLuint ubo[1];

public:
	~Renderer();

	void Setup(Display::Window* window);

	void Render(bool drawToScreen = true);

	const Resolution& GetRenderResolution() const;
	void SetRenderResolution(const Resolution& res);
	void SetRenderResolution(const int& x, const int& y);

	void SetWindowResolution(const int& x, const int& y);

	void SetFinalColorBuffer(GLuint cbuffer) { this->finalColorBuffer = cbuffer; }
	GLuint GetFinalColorBuffer() { return this->finalColorBuffer; }

	void SetRenderFBO(GLuint fbo) { this->renderFBO = fbo; }
	GLuint GetRenderFBO() { return this->renderFBO; }

	void SetupUniformBuffer(Graphics::Camera* camera);

	float GetRenderOption(RendererOptions option) { return renderOptions[option]; }
	void SetRenderOption(RendererOptions option, float val) { renderOptions[option] = val; }

	auto GetRenderFlags() { return &renderFlags; }
	void SetRenderFlags(uint32_t flags) { renderFlags = flags; }
	void SetRenderFlag(RendererSettings flag) { renderFlags |= flag; };
	void UnsetRenderFlag(RendererSettings flag) { renderFlags &= ~flag; };
	bool IsRenderFlagSet(RendererSettings flag) { return renderFlags & flag; }

private:

	Util::Dictionary<RendererOptions, float> renderOptions;

	uint32_t renderFlags = 0;

	Resolution renderResolution;
	Resolution windowResolution;

	Display::Window* window;

	GLuint finalColorBuffer = 0;
	GLuint renderFBO = 0;
};
}
