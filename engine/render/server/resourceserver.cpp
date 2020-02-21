#include "config.h"
#include "resourceserver.h"

namespace Render
{
//------------------------------------------------------------------------------
/**
*/
ResourceServer::ResourceServer()
{

}
//------------------------------------------------------------------------------
/**
*/
Ptr<TextureResource> ResourceServer::LoadTexture(const Util::String& filepath)
{
	if(!this->HasTextureNamed(filepath))
	{
		Ptr<TextureResource> texture = TextureResource::Create();
		texture->LoadTextureFile(filepath.AsCharPtr());
		this->textures.Add(filepath, texture);

		return texture;
	}
	else
	{
		return this->textures[filepath];
	}
}


//------------------------------------------------------------------------------
/**
*/
bool ResourceServer::HasTextureNamed(const Util::String& nName) const
{
	return this->textures.Contains(nName); 
}

//------------------------------------------------------------------------------
/**
*/
Ptr<MeshResources> ResourceServer::LoadMesh(const Util::String& filepath)
{
	if (!this->HasMeshNamed(filepath))
	{
		Ptr<MeshResources> mesh = MeshResources::Create();
		mesh->LoadMesh(filepath.AsCharPtr());
		this->meshes.Add(filepath, mesh);

		return mesh;
	}
	else
	{
		return this->meshes[filepath];
	}
}

//------------------------------------------------------------------------------
/**
*/
bool ResourceServer::HasMeshNamed(const Util::String& nName) const
{
	return this->meshes.Contains(nName);
}

}
