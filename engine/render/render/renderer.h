#pragma once
#include "core/app.h"
#include "foundation/math/mat4.h"
#include "core/singleton.h"

namespace Display { class Window; }
namespace Graphics { class Camera; }

namespace Render
{
struct Resolution
{
	GLint x;
	GLint y;
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

	const Resolution& GetRenderResolution() const;
	void SetRenderResolution(const Resolution& res);
	void SetRenderResolution(const int& x, const int& y);

	void SetupUniformBuffer(Graphics::Camera* camera);

private:
	
	Resolution renderResolution;

	Display::Window* window;
};
}
