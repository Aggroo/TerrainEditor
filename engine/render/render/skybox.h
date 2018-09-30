#pragma once
#include "application/game/entity.h"
#include "render/resources/textureresource.h"
#include "render/resources/shaderobject.h"

namespace Render
{
class Skybox : public Game::Entity
{
__DeclareClass(Skybox)
public:
	Skybox();
	~Skybox();

	void Activate();
	void Deactivate();
	virtual void Update();

	GLuint GetCubemapID() const { return cubemap->GetTextureID(); }
	Ptr<TextureResource> GetCubemap() const { return cubemap; }

private:
	void GenerateCube();

	GLuint vao;
	GLuint vbo;
	Util::Array<Util::String> cubeTextures;
	Util::Array<float> cubeVerts;

	Ptr<TextureResource> cubemap;
	Ptr<Render::ShaderObject> shader;
};
	
}
