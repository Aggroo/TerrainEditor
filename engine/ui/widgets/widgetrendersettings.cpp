#include "config.h"
#include "widgetrendersettings.h"
#include "render/server/frameserver.h"
#include "nfd.h"
#include "render/render/renderer.h"
#include <vector>
#include "foundation/math/math.h"

namespace UI
{
RenderSettings::RenderSettings(Display::Window* app) : Widget(app)
{
	this->title = "Render Settings";
	flags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar;
	isVisible = false;
	envTexPopup = false;
}

void RenderSettings::Update()
{
	if (isVisible)
	{
		if(ImGui::CollapsingHeader(this->title.AsCharPtr(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			Util::String label = "##EnvTex";
			Util::String dot = "..." + label;
			static std::vector<char*> tonemappingOptions = { "Off", "ACES", "Uncharted 2", "Reinhard" };
			const char* tonemapping_selection = tonemappingOptions[static_cast<unsigned int>(Render::Renderer::Instance()->GetRenderOption(Render::RendererOptions::OptionTonemapping))];

			const Util::String& texName = Render::FrameServer::Instance()->GetIBLPass()->GetHDRTexturePath();

			ImGui::LabelText("##texture", "Environment Map");
			if (ImGui::Button(dot.AsCharPtr()))
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

				ImGui::InputText(label.AsCharPtr(), (char*)s.AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
			}
			else
			{
				ImGui::InputText(label.AsCharPtr(), (char*)texName.AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
			}

			const auto renderOptionFunc = [this](const char* id, const char* text, Render::RendererOptions renderOption, char* tooltip = nullptr, float step = 0.1f)
			{
				float value = Render::Renderer::Instance()->GetRenderOption(renderOption);

				ImGui::PushID(id);
				ImGui::PushItemWidth(120);
				ImGui::BeginGroup();
				ImGui::LabelText(id, text);
				ImGui::InputFloat(id, &value, step);
				ImGui::EndGroup();
				ImGui::PopItemWidth();
				ImGui::PopID();

				// Only update if changed
				if (Render::Renderer::Instance()->GetRenderOption(renderOption) != value)
				{
					Render::Renderer::Instance()->SetRenderOption(renderOption, value);
				}

				if(tooltip != nullptr)
					RenderTooltip(tooltip);

			};

			if (ImGui::TreeNode("ToneMapping##tones"))
			{
				if (ImGui::BeginCombo("##TonemappingSettings", tonemapping_selection))
				{
					for (unsigned int i = 0; i < static_cast<unsigned int>(tonemappingOptions.size()); i++)
					{
						const auto is_selected = (tonemapping_selection == tonemappingOptions[i]);
						if (ImGui::Selectable(tonemappingOptions[i], is_selected))
						{
							tonemapping_selection = tonemappingOptions[i];
							Render::Renderer::Instance()->SetRenderOption(Render::RendererOptions::OptionTonemapping, static_cast<float>(i));
						}
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				renderOptionFunc("##tonemapping_option_1", "Exposure", Render::RendererOptions::OptionExposure);
				ImGui::SameLine(); renderOptionFunc("##tonemapping_option_2", "Gamma", Render::RendererOptions::OptionGamma);
				ImGui::Separator();

				ImGui::TreePop();
			}

			ModalWindows();
		}
	}
}

void RenderSettings::ModalWindows() 
{
	if (this->envTexPopup) { ImGui::OpenPopup("OpenEnvTexture"); }

	if (ImGui::BeginPopupModal("OpenEnvTexture", &this->envTexPopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("hdr;jpg,jpeg,tga,png", NULL, &outpath);

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
