#pragma once
#include "entitybase.h"
#include "render/resources/texturenode.h"
#include "render/resources/shaderobject.h"

namespace Game
{
class Entity : public Game::EntityBase
{
	__DeclareClass(Entity);
public:
	Entity();
	~Entity();

	void Activate();
	void Deactivate();

	virtual void Update();

	virtual void OnUI();

	void SetMesh(Util::String filename);
	Ptr<Render::MeshResources> GetMesh();

	void SetTextures(Util::String albedo, Util::String normal, Util::String metallic, Util::String roughness, Util::String ao = "Default");

	void SetShaders(Util::String vertexShader, Util::String fragmentShader, const char* name);
	Ptr<Render::ShaderObject> GetShader();

	void SetName(Util::String name);
	Util::String GetName();

	void SetEnvironmentMap(Ptr<Render::TextureResource> envmapID);
	Ptr<Render::TextureResource> GetEnvironmentMap();

private:

	Ptr<Render::TextureNode> textures;
	Ptr<Render::MeshResources> mesh;
	Ptr<Render::ShaderObject> shader;
	Ptr<Render::TextureResource> environmentMap;

	Util::String entityName;
};

}

