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
	splat,
};
class TextureNode : public Core::RefCounted
{
__DeclareClass(TextureNode)

public:
	TextureNode();
	~TextureNode();

	void AddTexture(TextureIndex index, const char * filename);
	void UpdateTexture(TextureIndex index, const char * filename) const;
	Ptr<TextureResource> GetTexture(TextureIndex index) const;
	void BindTextures() const;

private:

	Util::Dictionary<TextureIndex, Ptr<TextureResource>> textures;

};

}
