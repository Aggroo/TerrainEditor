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

private:

	//Name of the material
	Util::String name;

	///What frampasses this material and shader will be used in
	Util::Dictionary<Util::String, Ptr<ShaderObject>> framepasses;

	///This materials loaded textures
	Render::TextureNode textures;
};

} // namespace Render
