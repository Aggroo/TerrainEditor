#include "config.h"
#include "DeferredRenderer.h"
#include "GLFW/glfw3.h"
#include "render/window.h"
#include "shaderobject.h"
#include "lightnode.h"

namespace Renderer
{
DeferredRenderer::DeferredRenderer()
{
	fbo = 0;
	depthTexture = 0;
	ZERO_MEM(textures);
}

DeferredRenderer::~DeferredRenderer()
{
	if (fbo != 0) {
		glDeleteFramebuffers(1, &fbo);
	}

	if (textures[0] != 0) {
		glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(textures), textures);
	}

	if (depthTexture != 0) {
		glDeleteTextures(1, &depthTexture);
	}
}
}
