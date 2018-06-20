#include "config.h"
#include "texturenode.h"

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
	this->textures.Add(index, Math::TextureResource::Create());
	this->textures[index]->LoadTextureFile(filename);
}

void TextureNode::UpdateTexture(TextureIndex index, const char* filename) const
{
	if (this->textures.Contains(index))
	{
		this->textures[index]->LoadTextureFile(filename);
	}
}

Ptr<Math::TextureResource> TextureNode::GetTexture(TextureIndex index) const
{
	return this->textures[index];
}

	void TextureNode::BindTextures() const
{
	Util::Array<TextureIndex> indexArr = this->textures.KeysAsArray();

	for (int i = 0; i < indexArr.Size(); ++i)
	{
		this->textures[indexArr[i]]->bindTex((int)indexArr[i]);
	}
	
}

}
