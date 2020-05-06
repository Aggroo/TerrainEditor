#pragma once
#include "textureresource.h"

namespace Render
{
enum class TextureIndex
{
	albedo0,
	albedo1,
	albedo2,
	normal0,
	normal1,
	normal2,
	specular0,
	specular1,
	specular2,
	roughness0,
	roughness1,
	roughness2,
	ao0,
	ao1,
	ao2,
	splat,
	heightmap,
	environmentMap,
	irradiance,
	brdf
};
class TextureNode : public Core::RefCounted
{
__DeclareClass(TextureNode)

public:
	TextureNode();
	~TextureNode();

	void AddTexture(TextureIndex index, const char * filename);
	void AddTexture(TextureIndex index, const CreateTextureParameters & textureParams);
	void AddTexture(TextureIndex index, Ptr<TextureResource> texture);
	void UpdateTexture(TextureIndex index, const char * filename);
	Ptr<TextureResource> GetTexture(TextureIndex index) const;
	void BindTextures() const;
	void UnbindTextures() const;

private:

	/// The textures that have been added, uses TextureIndex as a key
	Util::Dictionary<TextureIndex, Ptr<TextureResource>> textures;

	/// Uses TextureIndex as key and value holds the uniform name in the shader
	Util::Dictionary<TextureIndex, Util::String> uniformTextureName;

};

}
