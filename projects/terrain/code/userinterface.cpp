#include "config.h"
#include "userinterface.h"
#include "CGlab.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "imgui_dock.h"
#include "nfd.h"
#include "render/resources/textureresource.h"
#include "imgui_internal.h"
#include "foundation/util/curve.hpp"

UserInterface::UserInterface(Example::CGLab* app)
{
	this->application = app;

	this->openPopup = false;
	this->texturePopup = false;
	heightSettings.texture = std::make_shared<Math::TextureResource>();

	// Setup style
	ImGuiStyle& style = ImGui::GetStyle();

	style.FrameRounding = 2.f;
	style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;


	foo[0].x = -1; // init data so editor knows to take it from here

	ImGui::InitDock();
}


UserInterface::~UserInterface()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void UserInterface::Run()
{

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
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::BeginMenu("Show"))
			{
				//if (ImGui::MenuItem("GUI Console", NULL)) { IO::Console::Instance()->Show(); }
				//if (ImGui::MenuItem("Statistics", NULL, &showStatistics)) {}
				//if (ImGui::MenuItem("Shader Debugger", NULL, &showShaderDebugger)) {}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	RenderTerrainSettings();
	ModalWindows();
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
		if (ImGui::MenuItem("Save Layout...")) { ImGui::InitDock(); }
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

void UserInterface::RenderDocks()
{
	const float toolbarWidth = 52.0f;
	const float toolButtonSize = 30.0f;
	
	ImGui::Begin("ToolBar", NULL,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBringToFrontOnFocus);
	{
		ImGui::SetWindowSize(ImVec2(toolbarWidth, (float)application->GetWindow()->GetHeight() - 16.0f), ImGuiSetCond_Always);
		ImGui::SetWindowPos(ImVec2(0.0f, 16.0f), ImGuiSetCond_Once);

		bool switched = false;
		if (ImGui::ImageButton((void*)this->icons.pointer->GetTextureID(), ImVec2(toolButtonSize, toolButtonSize)))
		{
			//Tools::ToolHandler::Instance()->SetSelectTool();
			switched = true;
		}
		if (ImGui::ImageButton((void*)this->icons.raiseTerrain->GetTextureID(), ImVec2(toolButtonSize, toolButtonSize)))
		{
			//Tools::ToolHandler::Instance()->SetTranslateTool();
			switched = true;
		}
		if (ImGui::ImageButton((void*)this->icons.flattenTerrain->GetTextureID(), ImVec2(toolButtonSize, toolButtonSize)))
		{
			//Tools::ToolHandler::Instance()->SetRotateTool();
			switched = true;
		}
		if (ImGui::ImageButton((void*)this->icons.paintTerrain->GetTextureID(), ImVec2(toolButtonSize, toolButtonSize)))
		{
			//this->currentTool = scaleTool;
			switched = true;
		}
		//if (ImGui::ImageButton((void*)this->entityToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
		//{
		//	//this->currentTool = entityTool;
		//	switched = true;
		//}
		//if (ImGui::ImageButton((void*)this->brushToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
		//{
		//	//this->currentTool = brushTool;
		//	switched = true;
		//}
		//if (ImGui::ImageButton((void*)this->polygonEditTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
		//{
		//	//this->currentTool = polygonEditTool;
		//	switched = true;
		//}

		//if (switched && application->hit.object != nullptr)
		//{
		//	Tools::ToolHandler::Instance()->CurrentTool()->UpdateTransform(application->hit.object->GetTransform());
		//}

		ImGui::End();
	}
	
	//ImGui::Begin("Dock", NULL,
	//	ImGuiWindowFlags_NoCollapse |
	//	ImGuiWindowFlags_NoMove |
	//	ImGuiWindowFlags_NoTitleBar |
	//	ImGuiWindowFlags_NoResize |
	//	ImGuiWindowFlags_NoScrollbar |
	//	ImGuiWindowFlags_NoScrollWithMouse |
	//	ImGuiWindowFlags_NoBringToFrontOnFocus);
	//{
	//	ImGui::SetWindowSize(ImVec2((float)application->GetWindow()->GetWidth()-52.0f, (float)application->GetWindow()->GetHeight()), ImGuiSetCond_Always);
	//	ImGui::SetWindowPos(ImVec2(52.0f, 5.0f), ImGuiSetCond_Once);

	//	ImGui::BeginDockspace();

	//	if (ImGui::BeginDock("Terrain")) {
	//		//ImGui::Image((ImTextureID)renderer->GetPositionTexture(), ImGui::GetWindowSize());
	//	}
	//	ImGui::EndDock();

	//	if (ImGui::BeginDock("Inspector")) {
	//		ImGui::Text("Who's your daddy?");
	//	}
	//	ImGui::EndDock();

	//	if (ImGui::BeginDock("test2")) {
	//		ImGui::Text("Who's your daddy?");
	//	}
	//	ImGui::EndDock();

	//	ImGui::EndDockspace();
	//}
	//ImGui::End();
	
}

void UserInterface::RenderTerrainSettings()
{
	bool open = true;
	ImGui::Begin("Terrain Settings", &open);
	{
		ImGui::Text("Settings");
		ImGui::BeginChild("TerrainSettings");
		{
			ImGui::BeginDockspace();

			if (ImGui::BeginDock("Heightmap", NULL, ImGuiWindowFlags_NoSavedSettings)) {

				if (!heightSettings.texName.IsEmpty())
				{
					Util::String s = heightSettings.texName.AsCharPtr();
					Util::Array<Util::String> path;

					s.Tokenize("/", path);

					s = path[path.Size() - 2] + "/" + path[path.Size() - 1];

					ImGui::InputText("##texture", (char*)s.AsCharPtr(), 256, ImGuiInputTextFlags_ReadOnly);
				}
				else
				{
					ImGui::InputText("##texture", (char*)heightSettings.texName.AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
				}
				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					this->texturePopup = true;
				}

				if (heightSettings.texName.IsEmpty())
				{
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				}
				ImGui::Indent(10);
				if (ImGui::Button("Generate", ImVec2(ImGui::GetWindowContentRegionWidth() - 20, 25)))
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

				ImGui::DragFloat("Width Multiplier", &heightSettings.widthMultiplier, 0.1f, 0.0f, 1000.f);
				ImGui::DragFloat("Height Multiplier", &heightSettings.heightMultiplier, 0.01f, 0.0f, 1000.f);
				if (ImGui::CollapsingHeader("Heightmap Image"))
				{
					if (!heightSettings.texName.IsEmpty())
					{
						Util::String s = heightSettings.texName.AsCharPtr();
						Util::Array<Util::String> path;

						s.Tokenize("/", path);

						s = path[path.Size() - 2] + "/" + path[path.Size() - 1];

						ImGui::InputText("##texture", (char*)s.AsCharPtr(), 256, ImGuiInputTextFlags_ReadOnly);
					}
					else
					{
						ImGui::InputText("##texture", (char*)heightSettings.texName.AsCharPtr(), 512, ImGuiInputTextFlags_ReadOnly);
					}
					ImGui::SameLine();
					if (ImGui::Button("..."))
					{
						this->texturePopup = true;
					}
					ImGui::Image((void*)heightSettings.texture->GetTextureID(), ImVec2(ImGui::GetWindowContentRegionWidth() - 5, ImGui::GetWindowContentRegionWidth() - 20));

				}
				if (ImGui::CollapsingHeader("Texture Settings"))
				{
					if(ImGui::DragFloat("UV Multiplier", &heightSettings.uvMultiplier, 0.01f, 0.0f, 1.f))
					{
						terrain->GetShader()->setupUniformFloat("uvMultiplier", heightSettings.uvMultiplier);
					}

				}
				if (ImGui::Curve("Height Curve", ImVec2(ImGui::GetWindowContentRegionWidth()-10 , 200), 10, foo))
				{
					// curve changed
				}


			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Perlin Noise", NULL, ImGuiWindowFlags_NoSavedSettings)) {

				if (ImGui::Button("Generate", ImVec2(ImGui::GetWindowContentRegionWidth() - 20, 25)))
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
			ImGui::EndDock();

			ImGui::EndDockspace();
		}
		ImGui::EndChild();

	}
	ImGui::End();

}

void UserInterface::ModalWindows()
{
	if (this->texturePopup) { ImGui::OpenPopup("OpenTexture"); }
	if (this->openPopup) { ImGui::OpenPopup("SaveTexture"); }
	if (ImGui::BeginPopupModal("OpenTexture", &this->texturePopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("jpg,jpeg;tga,png", NULL, &outpath);

		if (result == NFD_OKAY)
		{
			printf("path: %s\n", outpath);

			Util::String s = outpath;
			Util::Array<Util::String> path;
			s.ConvertBackslashes();
			heightSettings.texName = s.ExtractToEnd(s.FindStringIndex("resources")).AsCharPtr();

			heightSettings.texture->LoadTextureFile(outpath);

			this->texturePopup = false;
			free(outpath);
		}
		else if (result == NFD_CANCEL)
		{
			this->texturePopup = false;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
			assert(false);
			this->texturePopup = false;
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

			terrain->CreateTerrain(perlinSettings.name.AsCharPtr(), heightSettings.widthMultiplier, perlinSettings.scale, foo);

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
			this->texturePopup = false;
		}

		ImGui::EndPopup();
	}
}

void UserInterface::SetupImGuiStyle()
{
	ImGuiStyle * style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}