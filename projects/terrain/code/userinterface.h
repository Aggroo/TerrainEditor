#pragma once
#include <memory>
#include "application/game/terrain.h"
#include "foundation/util/string.h"
#include "application/game/perlinnoisegenerator.h"
#include "imgui.h"
#include "uisettings.h"

namespace Example{ class CGLab; }
namespace Render 
{
	enum class TextureIndex;
	class TextureResource; 
}

class UserInterface
{
public:
	UserInterface(Example::CGLab* app);
	~UserInterface();

	void Run();
	void ShowFileMenu();
	void RenderDocks();
	void ModalWindows();
	void SetupImGuiStyle() const;

	void GetImagePicker(Util::String texName, Render::TextureIndex index);
	Util::String GetStringFromTextureIndex(Render::TextureIndex index);
	void RenderTooltip(const char* text);

	void SetTerrain(Ptr<TerrainEditor::Terrain> terrain) { this->terrain = terrain; }

private:
	Example::CGLab* application;
	Ptr<TerrainEditor::Terrain> terrain;

	bool heightPopup;
	bool texturesPopup;
	bool openPopup;

	UI::HeightMapSettings heightSettings;
	UI::TextureSettings texSettings;
	UI::PerlinNoiseSettings perlinSettings;
	UI::UIIcons icons;
	UI::LightSettings light;
	Generator::PerlinNoiseGenerator p;

	ImVec2 foo[10];

};