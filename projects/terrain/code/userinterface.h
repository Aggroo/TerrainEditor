#pragma once
#include <memory>
#include "application/game/terrain.h"
#include "foundation/util/string.h"

namespace Example{ class CGLab; }
namespace Math { class TextureResource; }

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

	void SetTerrain(std::shared_ptr<TerrainEditor::Terrain> terrain) { this->terrain = terrain; }

private:
	Example::CGLab* application;
	std::shared_ptr<TerrainEditor::Terrain> terrain;

	bool texturePopup;
	bool openPopup;

	HeightMapSettings heightSettings;
};

