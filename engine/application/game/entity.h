#pragma once
#include "entitybase.h"
#include "render/resources/texturenode.h"
#include "render/resources/shaderobject.h"
#include "render/resources/model.h"
#include "ImGuizmo.h"

namespace Render
{
class Surface;
}

namespace Game
{

class Entity : public Game::EntityBase
{
	__DeclareClass(Entity);
public:
	struct transformGizmo
	{
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
		ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::MODE::WORLD;
		bool useSnap = false;
		float snap[3] = { 1.f, 1.f, 1.f };
	} gizmo;

public:
	Entity();
	~Entity();

	void Activate();
	void Deactivate();

	virtual void Update();

	virtual void OnUI();

	void SetMesh(Util::String filename);
	Ptr<Render::Model> GetMesh();

	void SetTextures(Util::String albedo, Util::String normal, Util::String metallic, Util::String roughness, Util::String ao = "Default");

	void SetSurface(Ptr<Render::Surface> surface);

	void SetShader(const char* name);
	Ptr<Render::ShaderObject> GetShader();

	void SetName(Util::String name);
	Util::String GetName();

	void SetIBLMaps(Ptr<Render::TextureResource> envmapID, Ptr<Render::TextureResource> irID, Ptr<Render::TextureResource> brdfID);
	Ptr<Render::TextureResource> GetEnvironmentMap();

private:

	Ptr<Render::Surface> surface;
	Ptr<Render::TextureNode> textures;
	Ptr<Render::Model> mesh;
	Ptr<Render::ShaderObject> shader;
	Ptr<Render::TextureResource> environmentMap, irradiance, brdf, ssao;

	Util::String entityName;

};

}

