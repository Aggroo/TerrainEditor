#include "config.h"
#include "widgetterrainsettings.h"
#include "foundation/util/curve.hpp"
#include "nfd.h"
#include "render/server/resourceserver.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{


TerrainSettings::TerrainSettings(Display::Window* app) : Widget(app), heightPopup(false), texturesPopup(false)
{
	Util::String title = ICON_FA_MOUNTAIN;
	title.Append(" Terrain Settings");
	this->title = title;
	this->heightSettings.texture = 0;
	this->curvePoints[0].x = -1; // init data so editor knows to take it from here
}

void TerrainSettings::Update()
{
	if (heightSettings.texName.IsEmpty())
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
	ImGui::Indent(10);
	if (ImGui::Button("Generate##heightmap", ImVec2(ImGui::GetWindowContentRegionWidth() - 20, 25)))
	{
		terrain->CreateTerrain(heightSettings.texName.AsCharPtr(), 
								heightSettings.size,
								heightSettings.widthMultiplier, 
								heightSettings.heightMultiplier, 
								curvePoints);
	}
	ImGui::Unindent(10);

	if (heightSettings.texName.IsEmpty())
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Heightmap Image", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!heightSettings.texName.IsEmpty())
		{
			Util::String s = heightSettings.texName.AsCharPtr();
			Util::Array<Util::String> path;

			s.Tokenize("/", path);

			s = path[path.Size() - 2] + "/" + path[path.Size() - 1];

			ImGui::LabelText("##texture", "Heightmap");
			ImGui::InputText("##texture", (char*)s.AsCharPtr(), 256, ImGuiInputTextFlags_ReadOnly);
		}
		else
		{
			ImGui::LabelText("##texture", "Heightmap");
			ImGui::InputText("##texture", (char*)heightSettings.texName.AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)heightSettings.texture, ImVec2(ImGui::GetContentRegionAvailWidth() - 5, ImGui::GetContentRegionAvailWidth() - 5)))
		{
			this->heightPopup = true;
		}

		ImGui::LabelText("##terrainwidth", "Width");
		ImGui::DragInt("##terrainwidth", &heightSettings.size, 1, 16, 16384);

		ImGui::LabelText("##widthmult", "Width Multiplier");
		ImGui::DragFloat("##widthmult", &heightSettings.widthMultiplier, 0.1f, 0.0f, 1000.f);
		ImGui::LabelText("##heightmult", "Height Multiplier");
		ImGui::DragFloat("##heightmult", &heightSettings.heightMultiplier, 0.01f, 0.0f, 1000.f);

		ImGui::LabelText("##heightscale", "Height Scale");
		if (ImGui::Curve("##heightscale", ImVec2(ImGui::GetWindowContentRegionWidth() - 10, 200), 10, curvePoints))
		{
			// curve changed
		}

	}
	if (ImGui::CollapsingHeader("Texture Settings"))
	{
		if (ImGui::TreeNode("Procedural Texturing Settings"))
		{
			if (ImGui::TreeNode("Blend 1"))
			{
				ImGui::LabelText("##slopeAngle", "Slope Angle");
				if (ImGui::DragFloat("##slopeAngle", &terrain->tsVar.slopeAngle, 0.01f, 0.0f, 1.f))
				{
				}

				ImGui::LabelText("##heightFalloff", "Height Falloff");
				if (ImGui::DragFloat("##heightFalloff", &terrain->tsVar.heightFalloff, 0.1f, 0.0f, 100.f))
				{
				}

				ImGui::LabelText("##height1", "Height");
				if (ImGui::DragFloat("##height1", &terrain->tsVar.height, 0.1f, 0.0f, 1000.f))
				{
				}

				ImGui::LabelText("##Hardness1", "Hardness1");
				if (ImGui::DragFloat("##Hardness1", &terrain->tsVar.hardness1, 0.1f, 0.0f, 30.f))
				{
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Blend 2"))
			{
				ImGui::LabelText("##slopeAngle2", "Slope Angle");
				if (ImGui::DragFloat("##slopeAngle2", &terrain->tsVar.slopeAngle2, 0.01f, 0.0f, 1.f))
				{
				}

				ImGui::LabelText("##heightFalloff2", "Height Falloff");
				if (ImGui::DragFloat("##heightFalloff2", &terrain->tsVar.heightFalloff2, 0.1f, 0.0f, 100.f))
				{
				}

				ImGui::LabelText("##height2", "Height");
				if (ImGui::DragFloat("##height2", &terrain->tsVar.height2, 0.1f, 0.0f, 1000.f))
				{
				}

				ImGui::LabelText("##Hardness2", "Hardness 2");
				if (ImGui::DragFloat("##Hardness2", &terrain->tsVar.hardness2, 0.1f, 0.0f, 30.f))
				{
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Texture 0 (R)"))
		{
			ImGui::LabelText("##Tex0UVMultiplier", "UV multiplier");
			if (ImGui::DragFloat("##Tex0UVMultiplier", &terrain->tsVar.texUv0Multiplier, 0.01f, 0.0f, 1.f))
			{
			}
			GetImagePicker(texSettings.tex0Name, Render::TextureIndex::albedo0);
			GetImagePicker(texSettings.normal0Name, Render::TextureIndex::normal0);
			GetImagePicker(texSettings.specular0Name, Render::TextureIndex::specular0);
			GetImagePicker(texSettings.roughness0Name, Render::TextureIndex::roughness0);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Texture 1 (G)"))
		{
			ImGui::LabelText("##Tex1UVMultiplier", "UV multiplier");
			if (ImGui::DragFloat("##Tex1UVMultiplier", &terrain->tsVar.texUv1Multiplier, 0.01f, 0.0f, 1.f))
			{
			}
			GetImagePicker(texSettings.tex1Name, Render::TextureIndex::albedo1);
			GetImagePicker(texSettings.normal1Name, Render::TextureIndex::normal1);
			GetImagePicker(texSettings.specular1Name, Render::TextureIndex::specular1);
			GetImagePicker(texSettings.roughness1Name, Render::TextureIndex::roughness1);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Texture 2 (B)"))
		{
			ImGui::LabelText("##Tex2UVMultiplier", "UV multiplier");
			if (ImGui::DragFloat("##Tex2UVMultiplier", &terrain->tsVar.texUv2Multiplier, 0.01f, 0.0f, 1.f))
			{
			}
			GetImagePicker(texSettings.tex2Name, Render::TextureIndex::albedo2);
			GetImagePicker(texSettings.normal2Name, Render::TextureIndex::normal2);
			GetImagePicker(texSettings.specular2Name, Render::TextureIndex::specular2);
			GetImagePicker(texSettings.roughness2Name, Render::TextureIndex::roughness2);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Splatmap"))
		{
			GetImagePicker(texSettings.splatName, Render::TextureIndex::splat);

			ImGui::TreePop();
		}
	}

	ModalWindow();
}

void TerrainSettings::GetImagePicker(Util::String texName, Render::TextureIndex index)
{
	Util::String label = "##Tex";
	label.AppendInt((int)index);
	if (!texName.IsEmpty())
	{
		Util::String s = texName.AsCharPtr();
		Util::Array<Util::String> path;

		s.Tokenize("/", path);

		s = path[path.Size() - 2] + "/" + path[path.Size() - 1];

		ImGui::LabelText(label.AsCharPtr(), GetStringFromTextureIndex(index).AsCharPtr());
		ImGui::InputText(label.AsCharPtr(), (char*)s.AsCharPtr(), 256, ImGuiInputTextFlags_ReadOnly);
	}
	else
	{
		ImGui::LabelText(label.AsCharPtr(), GetStringFromTextureIndex(index).AsCharPtr());
		ImGui::InputText(label.AsCharPtr(), (char*)texName.AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
	}
	ImGui::SameLine();
	Util::String dot = "..." + label;
	if (ImGui::ImageButton((void*)terrain->GetTextures()->GetTexture(index)->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth() - 10, ImGui::GetContentRegionAvailWidth() - 10)))
	{
		texSettings.chosenIndex = index;
		this->texturesPopup = true;
	}
}

Util::String TerrainSettings::GetStringFromTextureIndex(Render::TextureIndex index)
{
	switch (index)
	{
	case Render::TextureIndex::albedo0: return Util::String("Albedo");
	case Render::TextureIndex::albedo1: return Util::String("Albedo");
	case Render::TextureIndex::albedo2: return Util::String("Albedo");
	case Render::TextureIndex::normal0: return Util::String("Normal");
	case Render::TextureIndex::normal1: return Util::String("Normal");
	case Render::TextureIndex::normal2: return Util::String("Normal");
	case Render::TextureIndex::specular0: return Util::String("Specular/Metallic");
	case Render::TextureIndex::specular1: return Util::String("Specular/Metallic");
	case Render::TextureIndex::specular2: return Util::String("Specular/Metallic");
	case Render::TextureIndex::roughness0: return Util::String("Roughness");
	case Render::TextureIndex::roughness1: return Util::String("Roughness");
	case Render::TextureIndex::roughness2: return Util::String("Roughness");
	case Render::TextureIndex::splat: return Util::String("Splatmap");
	}
}

void TerrainSettings::ModalWindow()
{
	if (this->heightPopup) { ImGui::OpenPopup("OpenHeightTexture"); }
	if (this->texturesPopup) { ImGui::OpenPopup("OpenTexture"); }

	if (ImGui::BeginPopupModal("OpenHeightTexture", &this->heightPopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("jpg,jpeg,tga,png", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);

			Util::String s = outpath;
			Util::Array<Util::String> path;
			s.ConvertBackslashes();
			heightSettings.texName = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr();

			
			terrain->GetTextures()->UpdateTexture(Render::TextureIndex::heightmap, heightSettings.texName.AsCharPtr());
			heightSettings.texture = terrain->GetTextures()->GetTexture(Render::TextureIndex::heightmap)->GetTextureID();

			this->heightPopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			this->heightPopup = false;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->heightPopup = false;
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("OpenTexture", &this->texturesPopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("jpg,jpeg,tga,png", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);

			Util::String s = outpath;
			Util::Array<Util::String> path;
			s.ConvertBackslashes();

			switch (texSettings.chosenIndex)
			{
			case Render::TextureIndex::albedo0: texSettings.tex0Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::albedo1: texSettings.tex1Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::albedo2: texSettings.tex2Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::normal0: texSettings.normal0Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::normal1: texSettings.normal1Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::normal2: texSettings.normal2Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::specular0: texSettings.specular0Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::specular1: texSettings.specular1Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::specular2: texSettings.specular2Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::roughness0: texSettings.roughness0Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::roughness1: texSettings.roughness1Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::roughness2: texSettings.roughness2Name = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			case Render::TextureIndex::splat: texSettings.splatName = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr(); break;
			default:break;
			}

			terrain->GetTextures()->UpdateTexture(texSettings.chosenIndex, s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr());

			this->texturesPopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			this->texturesPopup = false;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->texturesPopup = false;
		}

		ImGui::EndPopup();
	}
}
}
