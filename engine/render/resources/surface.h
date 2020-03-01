#pragma once
//------------------------------------------------------------------------------
/**
    Class to handle the surface of a mesh

    (C) 2020 Anton Grönberg
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"

namespace Util 
{
class Variant;
}
namespace Render
{
struct MaterialParameter;
class TextureNode;
class Model;

class Surface : public Core::RefCounted
{
__DeclareClass(Surface);

public:
    Surface();
    ~Surface();

	void SetupShaderUniforms();

	Render::MaterialParameter* GetParameterByName(const Util::String& name);

	Ptr<Render::TextureNode> GetTextureList() { return textures; }

	void AddEntity(Ptr<Render::Model> entity);
	const Util::Array<Ptr<Render::Model>>& GetEntites();

private:
	friend class Material;
	friend class ResourceServer;

	void AddParameter(const Util::String &name, const Util::Variant &variable);

	Util::String name;
	Util::String filepath;

	///The material this surface is using
	Ptr<Render::Material> material;

	///This surface loaded textures
    Ptr<Render::TextureNode> textures;

	///Loaded material Parameters
	Util::Dictionary<Util::String, MaterialParameter*> parametersByName;
	Util::Array<MaterialParameter*> parameters;

	Util::Array<Ptr<Render::Model>> entites;

};
} // namespace Render
