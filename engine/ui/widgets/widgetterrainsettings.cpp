#include "config.h"
#include "widgetterrainsettings.h"
#include "foundation/util/curve.hpp"
#include "nfd.h"
#include "render/server/resourceserver.h"
#include "ui/imguiextra.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{


TerrainSettings::TerrainSettings(Display::Window* app) : Widget(app), heightPopup(false), texturesPopup(false), baseOrFilter(false)
{
	this->icon = ICON_FA_MOUNTAIN;
	this->title = "Terrain Settings";
	this->heightSettings.textures.Append(0);
	this->heightSettings.texNames.Append("");

	this->terrain = Terrain::Terrain::Create();
	this->terrain->Activate();
	this->terrain->CreateTerrain(heightSettings.size,
			heightSettings.widthMultiplier,
			heightSettings.heightMultiplier);

	this->heightSettings.textures[0] = this->terrain->GetSurface()->GetTextureList()->GetTexture(Render::TextureIndex::heightmap0)->GetTextureID();
}

void TerrainSettings::Update()
{
	if (!this->baseOrFilter)
	{
		ImGui::BeginButtonStyle(!this->baseOrFilter);
		ImGui::BeginChild("##buttontabs", ImVec2(ImGui::GetContentRegionAvail().x, 28.0f), false, ImGuiWindowFlags_NoScrollbar);
		ImGui::Indent(-1.0f);
		if (ImGui::Button("Base map##heightmapsbase"))
		{
			this->baseOrFilter = false;
		}
		ImGui::EndButtonStyle();
		ImGui::BeginButtonStyle(this->baseOrFilter);
		ImGui::SameLine(0.0f, 0.0f);
		if (ImGui::Button("Extra##heightmapsfilter"))
		{
			this->baseOrFilter = true;
		}
		ImGui::EndButtonStyle();
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::Indent(10);
		if (ImGui::Button("Update##heightmapgen", ImVec2(ImGui::GetWindowContentRegionWidth() - 20, 25)))
		{
			this->terrain->CreateTerrain(heightSettings.size,
					heightSettings.widthMultiplier,
					heightSettings.heightMultiplier);
		}
		ImGui::Unindent(10);

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Base Settings", ImGuiTreeNodeFlags_Leaf))
		{

			float total_w = ImGui::GetContentRegionAvail().x / 3.0f;

			ImGui::SetNextItemWidth(total_w);
			ImGui::LabelText("##terrainwidth", "Width");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::DragInt("##terrainwidth", &heightSettings.size, 16, 16, 16384))
			{
			}

			ImGui::SetNextItemWidth(total_w);
			ImGui::LabelText("##widthmult", "Width Multiplier");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::SliderFloat("##widthmult", &heightSettings.widthMultiplier, 0.0f, 10.f))
			{
			}

			ImGui::SetNextItemWidth(total_w);
			ImGui::LabelText("##heightmult", "Height Multiplier");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::SliderFloat("##heightmult", &heightSettings.heightMultiplier, 0.0f, 500.0f))
			{
				terrain->tsVar.heightScale = heightSettings.heightMultiplier;
			}

		}

		if (ImGui::CollapsingHeader("Texture Settings", ImGuiTreeNodeFlags_Leaf))
		{
			float total_w = ImGui::GetContentRegionAvail().x / 3.0f;
			if (ImGui::TreeNode("Procedural Texturing Settings"))
			{
				if (ImGui::TreeNode("Blend 1"))
				{
					ImGui::SetNextItemWidth(total_w);
					ImGui::LabelText("##slopeAngle", "Slope Angle");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::DragFloat("##slopeAngle", &terrain->tsVar.slopeAngle, 0.01f, 0.0f, 1.f))
					{
					}

					ImGui::SetNextItemWidth(total_w);
					ImGui::LabelText("##heightFalloff", "Height Falloff");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::DragFloat("##heightFalloff", &terrain->tsVar.heightFalloff, 0.1f, 0.0f, 100.f))
					{
					}

					ImGui::SetNextItemWidth(total_w);
					ImGui::LabelText("##height1", "Height");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::DragFloat("##height1", &terrain->tsVar.height, 0.1f, 0.0f, 1000.f))
					{
					}

					ImGui::SetNextItemWidth(total_w);
					ImGui::LabelText("##Hardness1", "Hardness1");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::DragFloat("##Hardness1", &terrain->tsVar.hardness1, 0.1f, 0.0f, 30.f))
					{
					}
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Blend 2"))
				{
					ImGui::SetNextItemWidth(total_w);
					ImGui::LabelText("##slopeAngle2", "Slope Angle");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::DragFloat("##slopeAngle2", &terrain->tsVar.slopeAngle2, 0.01f, 0.0f, 1.f))
					{
					}

					ImGui::SetNextItemWidth(total_w);
					ImGui::LabelText("##heightFalloff2", "Height Falloff");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::DragFloat("##heightFalloff2", &terrain->tsVar.heightFalloff2, 0.1f, 0.0f, 100.f))
					{
					}

					ImGui::SetNextItemWidth(total_w);
					ImGui::LabelText("##height2", "Height");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::DragFloat("##height2", &terrain->tsVar.height2, 0.1f, 0.0f, 1000.f))
					{
					}

					ImGui::SetNextItemWidth(total_w);
					ImGui::LabelText("##Hardness2", "Hardness 2");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::DragFloat("##Hardness2", &terrain->tsVar.hardness2, 0.1f, 0.0f, 30.f))
					{
					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Texture 0 (R)"))
			{
				ImGui::SetNextItemWidth(total_w);
				ImGui::LabelText("##Tex0UVMultiplier", "UV multiplier");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
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
				ImGui::SetNextItemWidth(total_w);
				ImGui::LabelText("##Tex1UVMultiplier", "UV multiplier");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
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
				ImGui::SetNextItemWidth(total_w);
				ImGui::LabelText("##Tex2UVMultiplier", "UV multiplier");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::DragFloat("##Tex2UVMultiplier", &terrain->tsVar.texUv2Multiplier, 0.01f, 0.0f, 1.f))
				{
				}
				GetImagePicker(texSettings.tex2Name, Render::TextureIndex::albedo2);
				GetImagePicker(texSettings.normal2Name, Render::TextureIndex::normal2);
				GetImagePicker(texSettings.specular2Name, Render::TextureIndex::specular2);
				GetImagePicker(texSettings.roughness2Name, Render::TextureIndex::roughness2);

				ImGui::TreePop();
			}
			//if (ImGui::TreeNode("Splatmap"))
			//{
			//	GetImagePicker(texSettings.splatName, Render::TextureIndex::splat);
			//
			//	ImGui::TreePop();
			//}
		}
	}
	else
	{
		ImGui::BeginButtonStyle(!this->baseOrFilter);
		ImGui::BeginChild("##buttontabs", ImVec2(ImGui::GetContentRegionAvail().x, 28.0f), false, ImGuiWindowFlags_NoScrollbar);
		ImGui::Indent(-1.0f);
		if (ImGui::Button("Base map##heightmapsbase"))
		{
			this->baseOrFilter = false;
		}
		ImGui::EndButtonStyle();
		ImGui::BeginButtonStyle(this->baseOrFilter);
		ImGui::SameLine(0.0f, 0.0f);
		if (ImGui::Button("Extra##heightmapsfilter"))
		{
			this->baseOrFilter = true;
		}
		ImGui::EndButtonStyle();
		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Heightmap Filters", ImGuiTreeNodeFlags_Leaf))
		{
			ImGui::BeginGroup();

			if (ImGui::Button("Add filter##heightmaps"))
			{
				terrain->heightPass->layerVars.numHeightmaps++;
				if (terrain->heightPass->layerVars.numHeightmaps > 5)
					terrain->heightPass->layerVars.numHeightmaps = 5;
				else
				{
					heightSettings.texNames.Append("");
					heightSettings.textures.Append(0);

					//terrain->tsVar.numHeightmaps++;
				}
				terrain->heightPass->Execute();
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove filter##heightmaps"))
			{
				terrain->heightPass->layerVars.numHeightmaps--;
				if (terrain->heightPass->layerVars.numHeightmaps < 1)
					terrain->heightPass->layerVars.numHeightmaps = 1;
				else
				{
					heightSettings.texNames.EraseIndex(heightSettings.texNames.Size() - 1);
					heightSettings.textures.EraseIndex(heightSettings.textures.Size() - 1);

					//terrain->tsVar.numHeightmaps--;
				}
				terrain->heightPass->Execute();
			}

			for (int i = 0; i < this->terrain->heightPass->layerVars.numHeightmaps; i++)
			{
				Util::String nodeName = "Heightmap ";
				nodeName.AppendInt(i + 1);
				nodeName.Append("##meshsettings");

				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 30.0f);
				if (ImGui::TreeNode(nodeName.AsCharPtr()))
				{
					nodeName = "##texture";
					nodeName.AppendInt(i);
					if (!heightSettings.texNames[i].IsEmpty())
					{
						Util::String s = heightSettings.texNames[i].AsCharPtr();
						Util::Array<Util::String> path;

						s.Tokenize("/", path);

						s = path[path.Size() - 2] + "/" + path[path.Size() - 1];

						ImGui::LabelText(nodeName.AsCharPtr(), "Filepath");
						ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 70.0f);
						ImGui::InputText(nodeName.AsCharPtr(), (char*)s.AsCharPtr(), 128, ImGuiInputTextFlags_ReadOnly);
					}
					else
					{
						ImGui::LabelText(nodeName.AsCharPtr(), "Filepath");
						ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 70.0f);
						ImGui::InputText(nodeName.AsCharPtr(), (char*)heightSettings.texNames[i].AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
					}
					ImGui::SameLine();
					if (ImGui::ImageButton((void*)heightSettings.textures[i], ImVec2(ImGui::GetContentRegionAvail().x - 35.0f, ImGui::GetContentRegionAvail().x - 35.0f)))
					{
						this->chosenHeightmap = i;
						this->heightPopup = true;
					}

					ImGui::Separator();

					if (i > 0)
					{
						float total_w = ImGui::GetContentRegionAvail().x / 3.0f;

						nodeName = "##masktext";
						nodeName.AppendInt(i);
						ImGui::SetNextItemWidth(total_w);
						ImGui::LabelText(nodeName.AsCharPtr(), "First Layer Mask");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						nodeName = "##mask";
						nodeName.AppendInt(i);
						if (ImGui::Checkbox(nodeName.AsCharPtr(), &heightSettings.useFirstLayerAsMask[i]))
						{
							terrain->heightPass->layerVars.useFirstLayerAsMask[i-1] = heightSettings.useFirstLayerAsMask[i] ? 1 : 0;
							terrain->heightPass->Execute();
						}

						nodeName = "##weighttext";
						nodeName.AppendInt(i);
						ImGui::SetNextItemWidth(total_w);
						ImGui::LabelText(nodeName.AsCharPtr(), "Layer Weight");
						ImGui::SameLine();
						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						nodeName = "##weight";
						nodeName.AppendInt(i);
						if (ImGui::SliderFloat(nodeName.AsCharPtr(), &terrain->heightPass->layerVars.layerStrength[i-1], 0.0f, 1.0f))
						{
							terrain->heightPass->Execute();
						}
					}

					ImGui::TreePop();
				}

			}
			ImGui::EndGroup();
		}
		
	}
	

	//if (heightSettings.texNames[0].IsEmpty())
	//{
	//	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	//	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	//}

	//if (heightSettings.texNames[0].IsEmpty())
	//{
	//	ImGui::PopItemFlag();
	//	ImGui::PopStyleVar();
	//}

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
	if (ImGui::ImageButton((void*)terrain->GetSurface()->GetTextureList()->GetTexture(index)->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth() - 10, ImGui::GetContentRegionAvailWidth() - 10)))
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
			heightSettings.texNames[chosenHeightmap] = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr();

			Render::TextureIndex texIndx;
			switch (chosenHeightmap)
			{
			case 0: texIndx = Render::TextureIndex::heightmap0; break;
			case 1: texIndx = Render::TextureIndex::heightmap1; break;
			case 2: texIndx = Render::TextureIndex::heightmap2; break;
			case 3: texIndx = Render::TextureIndex::heightmap3; break;
			case 4: texIndx = Render::TextureIndex::heightmap4; break;
			default:
				break;
			}
			
			terrain->heightPass->UpdateTexture(texIndx, heightSettings.texNames[chosenHeightmap].AsCharPtr());
			heightSettings.textures[chosenHeightmap] = terrain->heightPass->GetHeightmaps()->GetTexture(texIndx)->GetTextureID();

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

			terrain->GetSurface()->GetTextureList()->UpdateTexture(texSettings.chosenIndex, s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr());

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
