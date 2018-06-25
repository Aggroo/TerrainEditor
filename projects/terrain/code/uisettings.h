#pragma once
#include "render/resources/textureresource.h"
#include "render/server/resourceserver.h"

namespace UI
{
struct UIIcons
{
	UIIcons()
	{
		pointer = Render::ResourceServer::Instance()->LoadTexture("resources/textures/icons/pointer_terrain_icon.png");
		raiseTerrain = Render::ResourceServer::Instance()->LoadTexture("resources/textures/icons/raise_terrain_icon.png");
		flattenTerrain = Render::ResourceServer::Instance()->LoadTexture("resources/textures/icons/flatten_terrain_icon.png");
		paintTerrain = Render::ResourceServer::Instance()->LoadTexture("resources/textures/icons/paint_terrain_icon.png");
	}

	Ptr<Render::TextureResource> pointer;
	Ptr<Render::TextureResource> raiseTerrain;
	Ptr<Render::TextureResource> flattenTerrain;
	Ptr<Render::TextureResource> paintTerrain;

};
struct TextureSettings
{
	Util::String tex0Name;
	float tex0UvMultiplier = 0.1f;
	Util::String tex1Name;
	float tex1UvMultiplier = 0.1f;
	Util::String tex2Name;
	float tex2UvMultiplier = 0.1f;

	Util::String normal0Name;
	Util::String normal1Name;
	Util::String normal2Name;

	Util::String splatName;

	Render::TextureIndex chosenIndex;
};

struct HeightMapSettings
{
	Util::String texName;
	Ptr<Render::TextureResource> texture;
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
