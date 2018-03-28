#pragma once
#include "rasterizer.h"
#include <vector>
#include "textureresource.h"

namespace Math
{
class TextureNode
{
public:
	TextureNode();
	~TextureNode();

	void AddTexture(const char * filename);
	void BindTextures();

private:

    std::vector<std::shared_ptr<TextureResource>> textures;

};

}
