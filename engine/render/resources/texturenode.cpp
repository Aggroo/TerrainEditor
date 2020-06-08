#include "config.h"
#include "texturenode.h"
#include "render/server/resourceserver.h"

namespace Render
{
__ImplementClass(Render::TextureNode, 'TEXN', Core::RefCounted)


TextureNode::TextureNode()
{
}

TextureNode::~TextureNode()
{
}

void TextureNode::AddTexture(TextureIndex index, const char* filename)
{
	
	this->textures.Add(index, Render::ResourceServer::Instance()->LoadTexture(filename));
	//this->textures[index]->LoadTextureFile(filename);
}

void TextureNode::AddTexture(TextureIndex index, const CreateTextureParameters& textureParams)
{

	this->textures.Add(index, Render::ResourceServer::Instance()->LoadTexture(textureParams));
	//this->textures[index]->LoadTextureFile(filename);
}

void TextureNode::AddTexture(TextureIndex index, Ptr<TextureResource> texture)
{
	if (!this->textures.Contains(index))
	{
		this->textures.Add(index, texture);
	}
}

void TextureNode::UpdateTexture(TextureIndex index, const char* filename)
{
	if (this->textures.Contains(index))
	{
		CreateTextureParameters param = this->textures[index]->textureParameters;
		param.filename = filename;
		this->textures[index] = Render::ResourceServer::Instance()->LoadTexture(param);
	}
	else
	{
		this->textures.Add(index, Render::ResourceServer::Instance()->LoadTexture(filename));
	}
}

void TextureNode::UpdateTexture(TextureIndex index, Ptr<TextureResource> texture)
{
	this->textures[index] = texture;
}

Ptr<Render::TextureResource> TextureNode::GetTexture(TextureIndex index) const
{
	return this->textures[index];
}

void TextureNode::BindTextures() const
{
	Util::Array<TextureIndex> indexArr = this->textures.KeysAsArray();

	for (int i = 0; i < indexArr.Size(); ++i)
	{
		if(indexArr[i] == TextureIndex::environmentMap || indexArr[i] == TextureIndex::irradiance)
		{
			this->textures[indexArr[i]]->bindCubeTex((int)indexArr[i]);
		}
		else
		{
			this->textures[indexArr[i]]->bindTex((int)indexArr[i]);			
		}
	}
	
}

void TextureNode::UnbindTextures() const
{
	Util::Array<TextureIndex> indexArr = this->textures.KeysAsArray();

	for (int i = 0; i < indexArr.Size(); ++i)
	{
		this->textures[indexArr[i]]->unbindTex();
	}

}

}
