#include "config.h"
#include "userinterface.h"
#include "CGlab.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "imgui_dock.h"
#include "nfd.h"
#include "render/resources/textureresource.h"
#include "imgui_internal.h"

UserInterface::UserInterface(Example::CGLab* app)
{
	this->application = app;

	this->openPopup = false;
	this->texturePopup = false;
	heightSettings.texture = std::make_shared<Math::TextureResource>();

	// Setup style
	ImGui::StyleColorsDark();

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
	const float toolButtonSize = 32.0f;
	
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

		//bool switched = false;
		//if (ImGui::ImageButton((void*)this->selectToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
		//{
		//	Tools::ToolHandler::Instance()->SetSelectTool();
		//	switched = true;
		//}
		//if (ImGui::ImageButton((void*)this->translateToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
		//{
		//	Tools::ToolHandler::Instance()->SetTranslateTool();
		//	switched = true;
		//}
		//if (ImGui::ImageButton((void*)this->rotateToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
		//{
		//	Tools::ToolHandler::Instance()->SetRotateTool();
		//	switched = true;
		//}
		//if (ImGui::ImageButton((void*)this->scaleToolTextureHandle, ImVec2(toolButtonSize, toolButtonSize)))
		//{
		//	//this->currentTool = scaleTool;
		//	switched = true;
		//}
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
				ImGui::DragFloat("Width Multiplier", &heightSettings.widthMultiplier, 0.1f, 0.0f, 1000.f);
				ImGui::DragFloat("Height Multiplier", &heightSettings.heightMultiplier, 0.01f, 0.0f, 1000.f);
				if (ImGui::TreeNode("Heightmap Image"))
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
					ImGui::Image((void*)heightSettings.texture->GetTextureID(), ImVec2(ImGui::GetWindowContentRegionWidth() - 10, ImGui::GetWindowContentRegionWidth() - 10));

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Texture Settings"))
				{
					if(ImGui::DragFloat("UV Multiplier", &heightSettings.uvMultiplier, 0.01f, 0.0f, 1.f))
					{
						terrain->GetShader()->setupUniformFloat("uvMultiplier", heightSettings.uvMultiplier);
					}

					ImGui::TreePop();
				}

				if (heightSettings.texName.IsEmpty())
				{
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				}
				if (ImGui::Button("Generate"))
				{
					terrain->CreateTerrain(heightSettings.texName.AsCharPtr(), heightSettings.widthMultiplier, heightSettings.heightMultiplier);
				}
				if (heightSettings.texName.IsEmpty())
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Perlin Noise", NULL, ImGuiWindowFlags_NoSavedSettings)) {
				ImGui::Text("Who's your daddy?");
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
	if (ImGui::BeginPopupModal("OpenTexture", &this->texturePopup))
	{
		nfdchar_t* outpath;
		nfdresult_t result = NFD_OpenDialog("tga,png;jpg,jpeg", NULL, &outpath);

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
}
