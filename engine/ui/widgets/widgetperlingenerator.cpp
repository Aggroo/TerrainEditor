#include "config.h"
#include "widgetperlingenerator.h"
#include "nfd.h"

namespace UI
{
PerlinSettings::PerlinSettings(Display::Window* app) : Widget(app), openPopup(false)
{
	this->title = "Perlin Noise Settings";
}

void PerlinSettings::Update()
{
	if (ImGui::Button("Generate##Perlin", ImVec2(ImGui::GetWindowContentRegionWidth() - 20, 25)))
	{
		this->openPopup = true;
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Perlin Texture"))
	{
		ImGui::Image((void*)p.GetTexture()->GetTextureID(), ImVec2(ImGui::GetWindowContentRegionWidth() - 5, ImGui::GetWindowContentRegionWidth() - 20));
	}
	if (ImGui::CollapsingHeader("Generation Settings"))
	{
		ImGui::DragInt("Width", &perlinSettings.width, 1.0f, 1.f, 1000.f);
		ImGui::DragInt("Height", &perlinSettings.height, 1.0f, 1.f, 1000.f);
		ImGui::DragFloat("Scale", &perlinSettings.scale, 0.1f, 0.0f, 1000.f);
		ImGui::SliderInt("octaves", &perlinSettings.octaves, 1, 8);
		ImGui::DragFloat("Persistance", &perlinSettings.persistance, 0.01f, 0.0f, 1.f);
		ImGui::DragFloat("Lacunarity", &perlinSettings.lacunarity, 0.1f, 0.0001f, 1000.f);

	}
}

void PerlinSettings::ModalWindows()
{
	if (this->openPopup) { ImGui::OpenPopup("SaveTexture"); }

	if (ImGui::BeginPopupModal("SaveTexture", &this->openPopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_SaveDialog("jpg,jpeg", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);

			Util::String s = outpath;
			Util::Array<Util::String> path;
			s.ConvertBackslashes();
			perlinSettings.name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr();
			p.GenerateNoiseMap(perlinSettings.name.AsCharPtr(), perlinSettings.width, perlinSettings.height, perlinSettings.scale, perlinSettings.octaves, perlinSettings.persistance, perlinSettings.lacunarity);
			p.GetTexture()->LoadTextureFile(perlinSettings.name.AsCharPtr());

			//terrain->CreateTerrain(perlinSettings.name.AsCharPtr(), heightSettings.widthMultiplier, 150.f, foo);

			this->openPopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			this->openPopup = false;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->openPopup = false;
		}

		ImGui::EndPopup();
	}
}
}
