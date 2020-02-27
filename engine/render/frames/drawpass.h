#pragma once
#include <string>
#include "framepass.h"
#include "core/app.h"
#include "render/render/skybox.h"

namespace Render
{
class Material;

class DrawPass : public FramePass
{
__DeclareClass(DrawPass)
public:
	DrawPass();
	~DrawPass();

	//Get the OpenGL FBO
	GLuint& GetFrameBufferObject() { return this->frameBufferObject; }

	//Get this framebuffer objects render texture
	GLuint& GetTextureBuffer() { return this->buffer; }

	void AddMaterial(Material* mat) { this->materials.Append(mat); }

	//Binds the Framebuffer object
	void BindFrameBuffer();

	virtual void Setup();

	virtual void Execute();

	//Update the framebuffer resolution
	virtual void UpdateResolution();

protected:
	friend class Renderer;
	friend class FrameServer;

	//The FBO of the Pass
	GLuint frameBufferObject;

	//Texture buffer
	GLuint buffer;


private:
	//Ptr<Render::Skybox> skybox;

	Util::Array<Material*> materials;
};
}
