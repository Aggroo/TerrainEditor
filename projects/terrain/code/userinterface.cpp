#include "config.h"
#include "UserInterface.h"
#include "imgui_dock.h"
#include "CGlab.h"

UserInterface::UserInterface(Example::CGLab* app)
{
	this->application = app;
}


UserInterface::~UserInterface()
{
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
		//this->saveAsCheck = false;
	}
	if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))
	{
		//this->saveAsPopup = true;
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Layout"))
	{
		if (ImGui::MenuItem("Save Layout...")) { /*ImGui::SaveDock("engine/toolkit/particleeditor/layout/default.layout");*/ }
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
	if (ImGui::MenuItem("Quit", "Alt+F4")) { /*application->Shutdown(true);*/ }

}

void UserInterface::RenderDocks()
{
	const float toolbarWidth = 52.0f;
	const float toolButtonSize = 32.0f;

	ImGui::Begin("ToolBar", NULL,
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBringToFrontOnFocus);
	{
		ImGui::SetWindowSize(ImVec2(toolbarWidth, (float)application->GetWindow()->GetHeight() - 16.0f), ImGuiSetCond_Once);
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

}
