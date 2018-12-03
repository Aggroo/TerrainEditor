#include "config.h"
#include "userinterface.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "nfd.h"
#include "render/resources/textureresource.h"
#include "imgui_internal.h"
#include "foundation/util/curve.hpp"
#include "foundation/util/threadpool.h"
#include "render/render/renderer.h"
#include "application/basegamefeatures/entitymanager.h"
#include "render/server/lightserver.h"
#include "render/server/frameserver.h"
#include "render/frames/lightcullingpass.h"
#include <corecrt_io.h>

UserInterface::UserInterface(Example::Application* app)
{
	this->application = app;

	this->openPopup = false;
	this->heightPopup = false;
	this->texturesPopup = false;
	//this->terrainSettingsOpen = false;
	this->heightSettings.texture = Render::TextureResource::Create();
	// Setup style
	SetupImGuiStyle();


	foo[0].x = -1; // init data so editor knows to take it from here

}

UserInterface::~UserInterface()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UserInterface::Run()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 35));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - 35));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking;

	ImGui::Begin("Base", NULL, window_flags);

	ImGuiID dockSpace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockSpace_id);
	ImGui::SetNextWindowDockId(dockSpace_id, ImGuiCond_Once);

	RenderToolBar();
	RenderDocks();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowFileMenu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) { /*commandManager->Undo();*/ }
			if (ImGui::MenuItem("Redo", "CTRL+Y")) { /*commandManager->Redo();*/ }  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("Show Framebuffer:");
			ImGui::Separator();

			if (ImGui::MenuItem("FlatGeometryLit Color")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetTextureBuffer()); }
			if (ImGui::MenuItem("Depth")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetDepthPass()->GetTextureBuffer()); }
			if (ImGui::MenuItem("Linear Depth")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetDepthPass()->GetLinearDepthBuffer()); }
			if (ImGui::MenuItem("Light Tiles")) { Render::Renderer::Instance()->SetFinalColorBuffer(dynamic_cast<Render::LightCullingPass*>(Render::FrameServer::Instance()->GetLightCullingPass().get())->GetBuffer()); }
			if (ImGui::MenuItem("FlatGeometryLit Normals")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer()); }
			if (ImGui::MenuItem("FlatGeometryLit Specular")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetSpecularBuffer()); }
			if (ImGui::MenuItem("FlatGeometryLit Roughness")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetRoughnessBuffer()); }
			ImGui::Separator();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ModalWindows();

	ImGui::End();
}

void UserInterface::ShowFileMenu()
{
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		//openPopup = true;
	}
	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		//ImGui::InitDock();
	}
	if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))
	{
		//this->saveAsPopup = true;
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Layout"))
	{
		if (ImGui::MenuItem("Save Layout...")) {  }
		if (ImGui::MenuItem("Load Layout...")) { /*ImGui::LoadDock("engine/toolkit/particleeditor/layout/default.layout");*/ }
		ImGui::EndMenu();
	}

	//if (ImGui::BeginMenu("Options"))
	//{
	//	ImGui::EndMenu();
	//}
	//if (ImGui::BeginMenu("Colors"))
	//{
	//for (int i = 0; i < ImGuiCol_COUNT; i++)
	//	ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
	//ImGui::EndMenu();
	//}
	//if (ImGui::BeginMenu("Disabled", false)) // Disabled
	//{
	//	IM_ASSERT(0);
	//}
	//if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) { application->Shutdown(true); }

}

void UserInterface::RenderToolBar()
{
	const float toolbarHeight = 25.0f;
	const float toolButtonSize = 20.0f;

	ImGuiContext& g = *GImGui;
	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(g.Viewports[0]->Pos.x, g.Viewports[0]->Pos.y + 20.0f));
	ImGui::SetNextWindowSize(ImVec2(g.Viewports[0]->Size.x, g.NextWindowData.MenuBarOffsetMinVal.y + g.FontBaseSize + g.Style.FramePadding.y + toolbarHeight));

	ImGui::Begin("ToolBar", NULL,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoTitleBar);
	{

		bool switched = false;
		if (ImGui::ImageButton((void*)this->icons.pointer->GetTextureID(), ImVec2(toolButtonSize, toolButtonSize)))
		{
			//Tools::ToolHandler::Instance()->SetSelectTool();
			switched = true;
		}
		RenderTooltip("Select");
		ImGui::SameLine(0, 1);
		if (ImGui::ImageButton((void*)this->icons.raiseTerrain->GetTextureID(), ImVec2(toolButtonSize, toolButtonSize)))
		{
			//Tools::ToolHandler::Instance()->SetTranslateTool();
			switched = true;
		}
		RenderTooltip("Raise/Lower terrain");
		ImGui::SameLine(0, 1);
		if (ImGui::ImageButton((void*)this->icons.flattenTerrain->GetTextureID(), ImVec2(toolButtonSize, toolButtonSize)))
		{
			//Tools::ToolHandler::Instance()->SetRotateTool();
			switched = true;
		}
		RenderTooltip("Flatten terrain");
		ImGui::SameLine(0, 1);
		if (ImGui::ImageButton((void*)this->icons.paintTerrain->GetTextureID(), ImVec2(toolButtonSize, toolButtonSize)))
		{
			//this->currentTool = scaleTool;
			switched = true;
		}
		RenderTooltip("Paint terrain");

		ImGui::End();
	}
}

void UserInterface::RenderDocks()
{

	if (ImGui::Begin("Terrain", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		int width = (int)(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
		int height = (int)(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

		// Make sure we are pixel perfect
		width -= (width % 2 != 0) ? 1 : 0;
		height -= (height % 2 != 0) ? 1 : 0;
		ImGui::Image((ImTextureID)Render::Renderer::Instance()->GetFinalColorBuffer(), ImVec2(width, height));
	}
	ImGui::End();
	
	if (ImGui::Begin("Heightmap", NULL))
	{
		if (heightSettings.texName.IsEmpty())
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		ImGui::Indent(10);
		if (ImGui::Button("Generate##heightmap", ImVec2(ImGui::GetWindowContentRegionWidth() - 20, 25)))
		{
			terrain->CreateTerrain(heightSettings.texName.AsCharPtr(), heightSettings.widthMultiplier, heightSettings.heightMultiplier, foo);
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
			if (ImGui::ImageButton((void*)heightSettings.texture->GetTextureID(), ImVec2(ImGui::GetContentRegionAvailWidth() - 5, ImGui::GetContentRegionAvailWidth() - 5)))
			{
				this->heightPopup = true;
			}
	
			ImGui::LabelText("##widthmult", "Width Multiplier");
			ImGui::DragFloat("##widthmult", &heightSettings.widthMultiplier, 0.1f, 0.0f, 1000.f);
			ImGui::LabelText("##heightmult", "Height Multiplier");
			ImGui::DragFloat("##heightmult", &heightSettings.heightMultiplier, 0.01f, 0.0f, 1000.f);
	
			ImGui::LabelText("##heightscale", "Height Scale");
			if (ImGui::Curve("##heightscale", ImVec2(ImGui::GetWindowContentRegionWidth() - 10, 200), 10, foo))
			{
				// curve changed
			}
	
		}
		if (ImGui::CollapsingHeader("Texture Settings"))
		{
			if (ImGui::TreeNode("Texture 0 (R)"))
			{
				ImGui::LabelText("##Tex0UVMultiplier", "UV multiplier");
				if (ImGui::DragFloat("##Tex0UVMultiplier", &texSettings.tex0UvMultiplier, 0.01f, 0.0f, 1.f))
				{
					terrain->GetShader()->setupUniformFloat("tex0UvMultiplier", texSettings.tex0UvMultiplier);
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
				if (ImGui::DragFloat("##Tex1UVMultiplier", &texSettings.tex1UvMultiplier, 0.01f, 0.0f, 1.f))
				{
					terrain->GetShader()->setupUniformFloat("tex1UvMultiplier", texSettings.tex1UvMultiplier);
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
				if (ImGui::DragFloat("##Tex2UVMultiplier", &texSettings.tex2UvMultiplier, 0.01f, 0.0f, 1.f))
				{
					terrain->GetShader()->setupUniformFloat("tex2UvMultiplier", texSettings.tex2UvMultiplier);
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
	
	}
	ImGui::End();
	
	if (ImGui::Begin("Perlin Noise", NULL)) {
	
		if (ImGui::Button("Generate##Perlin", ImVec2(ImGui::GetWindowContentRegionWidth() - 20, 25)))
		{
	
			this->openPopup = true;
	
			//terrain->CreateTerrain(heightSettings.texName.AsCharPtr(), heightSettings.widthMultiplier, heightSettings.heightMultiplier);
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
	ImGui::End();
	
	if (ImGui::Begin("Inspector", NULL)) {
	
		
		if(Render::LightServer::Instance()->GetNumPointLights() > 0)
		{
			Render::LightServer::PointLight& light = Render::LightServer::Instance()->GetPointLightAtIndex(0);
			this->light.pos = light.position;
			this->light.col = light.color;
			this->light.radius = light.radiusAndPadding.x();
	
			if (ImGui::CollapsingHeader("Light"))
			{
				if (ImGui::DragFloat4("Position", (float*)&this->light.pos))
				{
					light.position = this->light.pos;
					Render::LightServer::Instance()->UpdatePointLightBuffer();
				}
				if (ImGui::DragFloat4("Color", (float*)&this->light.col, 0.01f))
				{
					light.color = this->light.col;
					Render::LightServer::Instance()->UpdatePointLightBuffer();
				}
				if (ImGui::DragFloat("Radius", &this->light.radius, 0.1f))
				{
					light.radiusAndPadding[0] = this->light.radius;
					Render::LightServer::Instance()->UpdatePointLightBuffer();
				}
			}
	
			auto entities = BaseGameFeature::EntityManager::Instance()->GetEntityList();
	
			for (auto it = entities.begin(); it != entities.end(); ++it)
			{
				it->second->OnUI();
			}
		}
	
	}
	ImGui::End();
	
}

void UserInterface::ModalWindows()
{
	if (this->heightPopup) { ImGui::OpenPopup("OpenHeightTexture"); }
	if (this->texturesPopup) { ImGui::OpenPopup("OpenTexture"); }
	if (this->openPopup) { ImGui::OpenPopup("SaveTexture"); }

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

			heightSettings.texture = Render::ResourceServer::Instance()->LoadTexture(heightSettings.texName.AsCharPtr());

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

			terrain->CreateTerrain(perlinSettings.name.AsCharPtr(), heightSettings.widthMultiplier, 150.f, foo);

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

void UserInterface::SetupImGuiStyle() const
{
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	float fontSize = 15.0f;
	float roundness = 2.0f;
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 text = ImVec4(0.76f, 0.77f, 0.8f, 1.0f);
	ImVec4 black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 backgroundVeryDark = ImVec4(0.08f, 0.086f, 0.094f, 1.00f);
	ImVec4 backgroundDark = ImVec4(0.117f, 0.121f, 0.145f, 1.00f);
	ImVec4 backgroundMedium = ImVec4(0.26f, 0.26f, 0.27f, 1.0f);
	ImVec4 backgroundLight = ImVec4(0.37f, 0.38f, 0.39f, 1.0f);
	ImVec4 highlightBlue = ImVec4(0.172f, 0.239f, 0.341f, 1.0f);
	ImVec4 highlightBlueActive = ImVec4(0.182f, 0.249f, 0.361f, 1.0f);
	ImVec4 highlightBlueHovered = ImVec4(0.202f, 0.269f, 0.391f, 1.0f);
	ImVec4 barBackground = ImVec4(0.078f, 0.082f, 0.09f, 1.0f);
	ImVec4 bar = ImVec4(0.164f, 0.180f, 0.231f, 1.0f);
	ImVec4 barHovered = ImVec4(0.411f, 0.411f, 0.411f, 1.0f);
	ImVec4 barActive = ImVec4(0.337f, 0.337f, 0.368f, 1.0f);

	// Spatial
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	//style.WindowMinSize		= ImVec2(160, 20);
	style.FramePadding = ImVec2(5, 5);
	style.ItemSpacing = ImVec2(6, 5);
	//style.ItemInnerSpacing	= ImVec2(6, 4);
	style.Alpha = 1.0f;
	style.WindowRounding = roundness;
	style.FrameRounding = roundness;
	style.PopupRounding = roundness;
	//style.IndentSpacing		= 6.0f;
	//style.ItemInnerSpacing	= ImVec2(2, 4);
	//style.ColumnsMinSpacing	= 50.0f;
	//style.GrabMinSize			= 14.0f;
	style.GrabRounding = roundness;
	//style.ScrollbarSize		= 12.0f;
	style.ScrollbarRounding = roundness;

	// Colors
	style.Colors[ImGuiCol_Text] = text;
	//style.Colors[ImGuiCol_TextDisabled]			= ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = backgroundDark;
	//style.Colors[ImGuiCol_ChildBg]				= ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
	style.Colors[ImGuiCol_Border] = black;
	//style.Colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = bar;
	style.Colors[ImGuiCol_FrameBgHovered] = highlightBlue;
	style.Colors[ImGuiCol_FrameBgActive] = highlightBlueHovered;
	style.Colors[ImGuiCol_TitleBg] = backgroundVeryDark;
	//style.Colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = bar;
	style.Colors[ImGuiCol_MenuBarBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_ScrollbarBg] = barBackground;
	style.Colors[ImGuiCol_ScrollbarGrab] = bar;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = barHovered;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = barActive;
	style.Colors[ImGuiCol_CheckMark] = white;
	style.Colors[ImGuiCol_SliderGrab] = bar;
	style.Colors[ImGuiCol_SliderGrabActive] = barActive;
	style.Colors[ImGuiCol_Button] = barActive;
	style.Colors[ImGuiCol_ButtonHovered] = highlightBlue;
	style.Colors[ImGuiCol_ButtonActive] = highlightBlueHovered;
	style.Colors[ImGuiCol_Header] = highlightBlue; // selected items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderHovered] = highlightBlueHovered; // hovered items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderActive] = highlightBlueActive;
	style.Colors[ImGuiCol_Separator] = backgroundLight;
	//style.Colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	//style.Colors[ImGuiCol_SeparatorActive]		= ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = backgroundMedium;
	style.Colors[ImGuiCol_ResizeGripHovered] = highlightBlue;
	style.Colors[ImGuiCol_ResizeGripActive] = highlightBlueHovered;
	//style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	//style.Colors[ImGuiCol_PlotLinesHovered]		= ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = highlightBlue; // Also used for progress bar
	style.Colors[ImGuiCol_PlotHistogramHovered] = highlightBlueHovered;
	style.Colors[ImGuiCol_TextSelectedBg] = highlightBlue;
	style.Colors[ImGuiCol_PopupBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_DragDropTarget] = backgroundLight;
	//style.Colors[ImGuiCol_ModalWindowDarkening]	= ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
}

void UserInterface::GetImagePicker(Util::String texName, Render::TextureIndex index)
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

Util::String UserInterface::GetStringFromTextureIndex(Render::TextureIndex index)
{
	switch (index)
	{
	case Render::TextureIndex::albedo0: return Util::String("Albedo");
	case Render::TextureIndex::albedo1: return Util::String("Albedo");
	case Render::TextureIndex::albedo2: return Util::String("Albedo");
	case Render::TextureIndex::normal0: return Util::String("Normal");
	case Render::TextureIndex::normal1: return Util::String("Normal");
	case Render::TextureIndex::normal2: return Util::String("Normal");
	case Render::TextureIndex::specular0: return Util::String("Specular");
	case Render::TextureIndex::specular1: return Util::String("Specular");
	case Render::TextureIndex::specular2: return Util::String("Specular");
	case Render::TextureIndex::roughness0: return Util::String("Roughness");
	case Render::TextureIndex::roughness1: return Util::String("Roughness");
	case Render::TextureIndex::roughness2: return Util::String("Roughness");
	case Render::TextureIndex::splat: return Util::String("Splatmap");
	}
}

void UserInterface::RenderTooltip(const char* text)
{
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(text);
}
