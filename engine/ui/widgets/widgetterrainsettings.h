#pragma once
#include "widget.h"
#include "render/resources/texturenode.h"
#include "render/terrain/terrain.h"

namespace Render
{
	class TextureResource;
}

namespace UI
{
struct TextureSettings
{

	Util::String tex0Name;
	Util::String tex1Name;
	Util::String tex2Name;

	Util::String normal0Name;
	Util::String normal1Name;
	Util::String normal2Name;

	Util::String specular0Name;
	Util::String specular1Name;
	Util::String specular2Name;

	Util::String roughness0Name;
	Util::String roughness1Name;
	Util::String roughness2Name;

	Util::String splatName;

	Render::TextureIndex chosenIndex;
};

struct HeightMapSettings
{
	Util::Array<Util::String> texNames;
	Util::Array<GLuint> textures;
	int size = 512;
	float widthMultiplier = 1.f;
	float heightMultiplier = 1.f;
	bool useFirstLayerAsMask[5] = { false, false, false, false, false };
};

class TerrainSettings : public Widget
{
public:

	explicit TerrainSettings(Display::Window* app);

	void Update() override;

	void SetTerrain(Ptr<Terrain::Terrain> terrain) { this->terrain = terrain; }

private:

	void GetImagePicker(Util::String texName, Render::TextureIndex index);
	Util::String GetStringFromTextureIndex(Render::TextureIndex index);

	void ModalWindow();

	Ptr<Terrain::Terrain> terrain;

	bool heightPopup;
	bool texturesPopup;
	bool baseOrFilter;

	UI::HeightMapSettings heightSettings;
	UI::TextureSettings texSettings;

	//The number of heightmaps you want to blend with. One base and X "filters"
	int chosenHeightmap;

};
}
