#include "config.h"
#include "resourceserver.h"

namespace Render
{
ResourceServer::ResourceServer()
{

}

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

Ptr<TextureResource> ResourceServer::GetTexture(const Util::String& filepath)
{
	if (this->HasTextureNamed(filepath))
	{
		return this->textures[filepath];
	}
	else
	{
		return this->LoadTexture(filepath);
	}
}

bool ResourceServer::HasTextureNamed(const Util::String& nName) const
{
	return this->textures.Contains(nName); 
}

}
