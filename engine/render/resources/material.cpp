//------------------------------------------------------------------------------
//  material.cc
//  (C) 2020 Anton Grönberg
//------------------------------------------------------------------------------
#include "config.h"
#include "material.h"
#include "render/server/frameserver.h"
#include "render/server/shaderserver.h"

namespace Render
{
__ImplementClass(Render::Material, 'MATL', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
Material::Material()
{

}

//------------------------------------------------------------------------------
/**
*/
Material::~Material()
{

}

//------------------------------------------------------------------------------
/**
*/
void Material::SetFramePass(const Util::String& framepass, const Util::String& shader)
{
	if (FrameServer::Instance()->HasPassNamed(framepass))
	{
		auto drawpass = FrameServer::Instance()->GetFramePass(framepass).downcast<DrawPass>();

		//Make sure we can actually cast
		assert(drawpass != nullptr);

		drawpass->AddMaterial(this);
		this->framepasses.Add(framepass, Render::ShaderServer::Instance()->GetShader(shader.AsCharPtr()));
	}
	else
	{
		printf("ERROR Material::SetFramePass : Cannot find pass named %s!\n", framepass.AsCharPtr());
		assert(false);
	}
}

//------------------------------------------------------------------------------
/**
*/
MaterialParameter* Material::GetParameterByName(const Util::String& name)
{
	if (this->parametersByName.Contains(name))
	{
		return this->parametersByName[name];
	}
	else
	{
		printf("ERROR Material::GetParameterByName: Could not find parameter with name %s!", name.AsCharPtr());

	#ifdef _DEBUG
		assert(false);
	#endif // _DEBUG

		return nullptr;
	}
}

//------------------------------------------------------------------------------
/**
*/
void Material::AddParameter(const Util::String& name, const Util::Variant& variable)
{
	if (variable.GetType() == Util::Variant::Type::String)
	{
		//HACK: Since we're loading a string, we're probably loading a texture
		//This should probably be done in some other way
		this->textures.AddTexture(Material::TextureIndexFromString(name.AsCharPtr()), variable.GetString().AsCharPtr());
	}
	else
	{
		MaterialParameter* param = new MaterialParameter();
		param->name = name;
		param->var = variable;
		this->parametersByName.Add(name, param);
		parameters.Append(param);
	}
}

//------------------------------------------------------------------------------
/**
*/
TextureIndex Material::TextureIndexFromString(const Util::String& parameter)
{
	if (parameter == TERRAIN_SEMANTIC_ALBEDOMAP || parameter == TERRAIN_SEMANTIC_TERRAINALBEDO(0))
	{
		return TextureIndex::albedo0;
	}
	else if (parameter == TERRAIN_SEMANTIC_TERRAINALBEDO(1))
	{
		return TextureIndex::albedo1;
	}
	else if (parameter == TERRAIN_SEMANTIC_TERRAINALBEDO(2))
	{
		return TextureIndex::albedo2;
	}
	else if (parameter == TERRAIN_SEMANTIC_NORMALMAP || parameter == TERRAIN_SEMANTIC_TERRAINNORMAL(0))
	{
		return TextureIndex::normal0;
	}
	else if (parameter == TERRAIN_SEMANTIC_TERRAINNORMAL(1))
	{
		return TextureIndex::normal1;
	}
	else if (parameter == TERRAIN_SEMANTIC_TERRAINNORMAL(2))
	{
		return TextureIndex::normal2;
	}
	else if (parameter == TERRAIN_SEMANTIC_SPECULARMAP || parameter == TERRAIN_SEMANTIC_TERRAINSPECULAR(0))
	{
		return TextureIndex::specular0;
	}
	else if (parameter == TERRAIN_SEMANTIC_TERRAINSPECULAR(1))
	{
		return TextureIndex::specular1;
	}
	else if (parameter == TERRAIN_SEMANTIC_TERRAINSPECULAR(2))
	{
		return TextureIndex::specular2;
	}
	else if (parameter == TERRAIN_SEMANTIC_ROUGHNESSMAP || parameter == TERRAIN_SEMANTIC_TERRAINROUGHNESS(0))
	{
		return TextureIndex::roughness0;
	}
	else if (parameter == TERRAIN_SEMANTIC_TERRAINROUGHNESS(1))
	{
		return TextureIndex::roughness1;
	}
	else if (parameter == TERRAIN_SEMANTIC_TERRAINROUGHNESS(2))
	{
		return TextureIndex::roughness2;
	}
	else
	{
		_assert2(false, "ERROR: Texture index name does not exist in semantics!");
		return TextureIndex::albedo0;
	}
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ShaderObject> Material::GetShader(const Util::String& pass)
{
	return this->framepasses[pass];
}

} // namespace Render