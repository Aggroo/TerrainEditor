#pragma once
//------------------------------------------------------------------------------
/**
    Class to handle the different materials in the engine

    (C) 2020 Anton Grönberg
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "foundation/util/variant.h"
#include "render/resources/texturenode.h"
#include "render/resources/shaderobject.h"
#include "render/resources/surface.h"

namespace Render
{

struct MaterialParameter
{
	Util::String name;
	Util::Variant var;
};

class Material : public Core::RefCounted
{
__DeclareClass(Material)

public:
	Material();
	~Material();
	
	void SetFramePass(const Util::String& framepass, const Util::String& shader);

	MaterialParameter* GetParameterByName(const Util::String& name);
	void AddParameter(const Util::String& name, const Util::Variant& variable);

	static TextureIndex TextureIndexFromString(const Util::String& parameter);
	Util::String UniformNameFromString(const Util::String& parameter);

	Util::Array<Ptr<Surface>>& SurfaceList() { return this->surfaces; };

	Ptr<ShaderObject> GetShader(const Util::String& pass);
	Util::Array<Ptr<ShaderObject>> GetShaders();

	void SetName(const Util::String& name) { this->name = name; }
	const Util::String& GetName() { return name; }

	void SetupUniformParameters();

private:

	friend class ResourceServer;

	///Name of the material
	Util::String name;

	///What frampasses this material and shader will be used in
	Util::Dictionary<Util::String, Ptr<ShaderObject>> framepasses;

	///This materials loaded textures
	Ptr<Render::TextureNode> textures;

	///Loaded material Parameters
	Util::Dictionary<Util::String, MaterialParameter*> parametersByName;
	Util::Array<MaterialParameter*> parameters;

	/// all surfaces that currently use this material
	Util::Array<Ptr<Surface>> surfaces;

};

} // namespace Render
