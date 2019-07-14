#pragma once
#include "widget.h"
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

class Toolbar : public Widget
{
public:

	Toolbar(Display::Window* app);

	void Update() override;
private:

	UI::UIIcons icons;
};
}