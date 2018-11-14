#pragma once
#include "core/singleton.h"
#include "core/refcounted.h"
#include "render/resources/textureresource.h"

namespace Render
{
class ResourceServer
{
__DeclareSingleton(ResourceServer)

public:
	///Returns a texture if it exists, otherwise loads it and then returns the new texture
	Ptr<TextureResource> LoadTexture(const Util::String& filepath);
	Ptr<TextureResource> GetTexture(const Util::String& filepath);
	bool HasTextureNamed(const Util::String& nName) const;

private:

	///This contains all textures.
	///Key must be unique to each texture. the key is the texture name loaded
	Util::Dictionary<Util::String, Ptr<TextureResource>> textures;

};	
}
