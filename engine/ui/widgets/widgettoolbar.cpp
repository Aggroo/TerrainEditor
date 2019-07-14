#include "config.h"
#include "widgettoolbar.h"
#include "imgui_internal.h"

namespace UI
{

Toolbar::Toolbar(Display::Window* app) : Widget(app)
{
	this->title = "Toolbar";
	this->flags = ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoTitleBar;

	callbackBegin = [this]() {
		const float toolbarHeight = 25.0f;
		ImGuiContext& g = *GImGui;
		g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
		ImGui::SetNextWindowPos(ImVec2(g.Viewports[0]->Pos.x, g.Viewports[0]->Pos.y + 20.0f));
		ImGui::SetNextWindowSize(ImVec2(g.Viewports[0]->Size.x, g.NextWindowData.MenuBarOffsetMinVal.y + g.FontBaseSize + g.Style.FramePadding.y + toolbarHeight)); 
	};
	

}


void Toolbar::Update()
{
	const float toolButtonSize = 20.0f;

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
	
}

}
