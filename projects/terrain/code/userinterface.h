#pragma once
#include <memory>
#include "application/game/terrain.h"
#include "foundation/util/string.h"
#include "application/game/perlinnoisegenerator.h"

namespace Example{ class CGLab; }
namespace Math { class TextureResource; }

struct UIIcons
{
	UIIcons()
	{
		pointer = std::make_shared<Math::TextureResource>();
		pointer->LoadTextureFile("resources/textures/icons/pointer_terrain_icon.png");
		raiseTerrain = std::make_shared<Math::TextureResource>();
		raiseTerrain->LoadTextureFile("resources/textures/icons/raise_terrain_icon.png");
		flattenTerrain = std::make_shared<Math::TextureResource>();
		flattenTerrain->LoadTextureFile("resources/textures/icons/flatten_terrain_icon.png");
		paintTerrain = std::make_shared<Math::TextureResource>();
		paintTerrain->LoadTextureFile("resources/textures/icons/paint_terrain_icon.png");
	}

	std::shared_ptr<Math::TextureResource> pointer;
	std::shared_ptr<Math::TextureResource> raiseTerrain;
	std::shared_ptr<Math::TextureResource> flattenTerrain;
	std::shared_ptr<Math::TextureResource> paintTerrain;

};

struct HeightMapSettings
{
	Util::String texName;
	std::shared_ptr<Math::TextureResource> texture;
	float widthMultiplier = 1.f;
	float heightMultiplier = 1.f;

	float uvMultiplier = 0.1f;
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

class UserInterface
{
public:
	UserInterface(Example::CGLab* app);
	~UserInterface();

	void Run();
	void ShowFileMenu();
	void RenderDocks();
	void RenderTerrainSettings();
	void ModalWindows();
	void SetupImGuiStyle();

	void SetTerrain(std::shared_ptr<TerrainEditor::Terrain> terrain) { this->terrain = terrain; }

private:
	Example::CGLab* application;
	std::shared_ptr<TerrainEditor::Terrain> terrain;

	bool texturePopup;
	bool openPopup;

	HeightMapSettings heightSettings;
	PerlinNoiseSettings perlinSettings;
	UIIcons icons;
	Generator::PerlinNoiseGenerator p;

};

