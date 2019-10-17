#pragma once
#include "widget.h"
#include "application/game/perlinnoisegenerator.h"

namespace UI
{
struct PerlinNoiseSettings
{
	Util::String name;
	int size = 100;
	float scale = 1.f;
	int octaves = 1;
	float persistance = 0.5f;
	float lacunarity = 5.f;
};

class PerlinSettings : public Widget
{
public:
	PerlinSettings(Display::Window* app);

	void Update() override;

private:

	void ModalWindows();

	bool openPopup;
	UI::PerlinNoiseSettings perlinSettings;
	Generator::PerlinNoiseGenerator p;
};
}
