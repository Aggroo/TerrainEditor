#include "config.h"
#include "widgetrendersettings.h"
#include "render/server/frameserver.h"
#include "nfd.h"

namespace UI
{
RenderSettings::RenderSettings(Display::Window* app) : Widget(app)
{
	this->title = "Render Settings";
	isVisible = false;
	envTexPopup = false;
}

void RenderSettings::Update()
{
	Util::String label = "##EnvTex";
	Util::String dot = "..." + label;

	const Util::String& texName = Render::FrameServer::Instance()->GetIBLPass()->GetHDRTexturePath();

	ImGui::LabelText(label.AsCharPtr(), "Environment Map");
	if (ImGui::Button(dot.AsCharPtr(), ImVec2(ImGui::GetContentRegionAvailWidth() - 10, ImGui::GetContentRegionAvailWidth() - 10)))
	{
		this->envTexPopup = true;
	}
	ImGui::SameLine();
	if (!texName.IsEmpty())
	{
		Util::String s = texName.AsCharPtr();
		Util::Array<Util::String> path;

		s.Tokenize("/", path);

		s = path[path.Size() - 2] + "/" + path[path.Size() - 1];

		ImGui::InputText(label.AsCharPtr(), (char*)s.AsCharPtr(), 256, ImGuiInputTextFlags_ReadOnly);
	}
	else
	{
		ImGui::InputText(label.AsCharPtr(), (char*)texName.AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
	}
}

void RenderSettings::ModalWindows() 
{
	if (this->envTexPopup) { ImGui::OpenPopup("OpenEnvTexture"); }

	if (ImGui::BeginPopupModal("OpenEnvTexture", &this->envTexPopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("jpg,jpeg,tga,png,hdr", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);

			Util::String s = outpath;
			Util::Array<Util::String> path;
			s.ConvertBackslashes();
			
			Render::FrameServer::Instance()->GetIBLPass()->UpdateIBL(s.ExtractToEnd(s.FindStringIndex("resources")));

			this->envTexPopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			this->envTexPopup = false;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->envTexPopup = false;
		}

		ImGui::EndPopup();
	}
}

}
