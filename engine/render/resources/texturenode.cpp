#include "config.h"
#include "texturenode.h"

namespace Math
{
TextureNode::TextureNode()
{
}

TextureNode::~TextureNode()
{
}

void TextureNode::AddTexture(const char* filename)
{
	this->textures.push_back(std::make_shared<Math::TextureResource>());
	this->textures[this->textures.size() - 1]->LoadTextureFile(filename);
}

void TextureNode::BindTextures()
{
	for (int i = 0; i < this->textures.size(); ++i)
		this->textures[i]->bindTex(i);
}

}
