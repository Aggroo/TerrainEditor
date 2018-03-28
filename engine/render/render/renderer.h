#pragma once
#include "core/app.h"
#include "foundation/math/mat4.h"

namespace Display { class Window; }
namespace Graphics { class Camera; }

namespace Render
{
struct Resolution
{
	int x;
	int y;
};

class Renderer
{
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
	static Renderer* Instance()
	{
		static Renderer instance;
		return &instance;
	}
	~Renderer();

	void Setup(Display::Window* window);

	Resolution GetRenderResolution();
	void SetRenderResolution(const Resolution& res);
	void SetRenderResolution(const int& x, const int& y);

	void SetupBufferBlock(Graphics::Camera* camera);

private:
	Renderer();
	
	Resolution renderResolution;

	Display::Window* window;
};
}
