#pragma once
#include "render/resources/textureresource.h"

namespace UI
{
struct UIIcons
{
	UIIcons()
	{
		pointer = Math::TextureResource::Create();
		pointer->LoadTextureFile("resources/textures/icons/pointer_terrain_icon.png");
		raiseTerrain = Math::TextureResource::Create();
		raiseTerrain->LoadTextureFile("resources/textures/icons/raise_terrain_icon.png");
		flattenTerrain = Math::TextureResource::Create();
		flattenTerrain->LoadTextureFile("resources/textures/icons/flatten_terrain_icon.png");
		paintTerrain = Math::TextureResource::Create();
		paintTerrain->LoadTextureFile("resources/textures/icons/paint_terrain_icon.png");
	}

	Ptr<Math::TextureResource> pointer;
	Ptr<Math::TextureResource> raiseTerrain;
	Ptr<Math::TextureResource> flattenTerrain;
	Ptr<Math::TextureResource> paintTerrain;

};
struct TextureSettings
{
	Util::String tex0Name;
	float tex0UvMultiplier = 0.1f;
	Util::String tex1Name;
	float tex1UvMultiplier = 0.1f;
	Util::String tex2Name;
	float tex2UvMultiplier = 0.1f;
	Util::String splatName;

	Render::TextureIndex chosenIndex;
};

struct HeightMapSettings
{
	Util::String texName;
	std::shared_ptr<Math::TextureResource> texture;
	float widthMultiplier = 1.f;
	float heightMultiplier = 1.f;

};



struct PerlinNoiseSettings
{
	Util::String name;
	int width = 100;
	int height = 100;
	float scale = 1.f;
	int octaves = 1;
	float persistance = 0.5f;
	float lacunarity = 5.f;
};
}
