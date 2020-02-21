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
	///Returns a texture if it exists, otherwise loads it and then returns the new textureResource
	Ptr<TextureResource> LoadTexture(const Util::String& filepath);
	///Check if Texture is loaded
	bool HasTextureNamed(const Util::String& nName) const;

	///Returns a mesh if it exists, otherwise loads it and then returns the new meshResource
	Ptr<MeshResources> LoadMesh(const Util::String& filepath);
	///Check if mesh is loaded
	bool HasMeshNamed(const Util::String& nName) const;

private:

	///This contains all textures.
	///Key must be unique to each texture. the key is the texture name loaded
	Util::Dictionary<Util::String, Ptr<TextureResource>> textures;

	///This contains all meshes.
	///Key must be unique to each mesh. the key is the mesh name loaded
	Util::Dictionary<Util::String, Ptr<MeshResources>> meshes;

};	
}
