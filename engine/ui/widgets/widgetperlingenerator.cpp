#include "config.h"
#include "widgetperlingenerator.h"
#include "nfd.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{
PerlinSettings::PerlinSettings(Display::Window* app) : Widget(app), openPopup(false)
{
	Util::String title = ICON_FA_TOOLS;
	title.Append(" Perlin Noise Settings");
	this->title = title;
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
		ImGui::DragInt("Width and height", &perlinSettings.size, 1.0f, 1.f, 1000.f);
		ImGui::DragFloat("Scale", &perlinSettings.scale, 0.1f, 0.0f, 1000.f);
		ImGui::SliderInt("Octaves", &perlinSettings.octaves, 1, 8);
		ImGui::DragFloat("Persistance", &perlinSettings.persistance, 0.01f, 0.0f, 1.f);
		ImGui::DragFloat("Frequency", &perlinSettings.lacunarity, 0.1f, 0.0001f, 1000.f);

	}

	ModalWindows();
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
			p.GenerateNoiseMap(perlinSettings.name.AsCharPtr(), perlinSettings.size, perlinSettings.size, perlinSettings.scale, perlinSettings.octaves, perlinSettings.persistance, perlinSettings.lacunarity);
			p.GetTexture()->LoadTextureFile(perlinSettings.name.AsCharPtr());

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
