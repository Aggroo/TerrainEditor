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
	void Update();

	void LoadCubemap(Util::Array<Util::String> texures);

private:

	void GenerateCube();

	GLuint cubemapID;
	GLuint vao;
	GLuint vbo;
	Util::Array<Util::String> cubeTextures;
	Util::Array<float> cubeVerts;

	Ptr<Math::ShaderObject> shader;
};
	
}
