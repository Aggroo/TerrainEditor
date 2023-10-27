#include "config.h"
#include "widgetmenubar.h"
#include "render/render/renderer.h"
#include "render/server/frameserver.h"
#include "render/window.h"
#include "ui/uiserver.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{
MenuBar::MenuBar(Display::Window* app) : Widget(app)
{
	this->isWindow = false;
	this->title = "Menubar";
}

void MenuBar::Update()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowFileMenu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) { CommandManager::Instance()->Undo(); }
			if (ImGui::MenuItem("Redo", "CTRL+Y")) { CommandManager::Instance()->Redo(); }  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			Util::String terrainSettings = "Terrain Settings";
			Util::String renderSettings = "Render Settings";

			if (ImGui::MenuItem("Terrain Settings", "P")) { UI::UIServer::Instance()->SetWidgetVisibility(terrainSettings, true); }
			if (ImGui::MenuItem("Render Settings", "O")) { UI::UIServer::Instance()->SetWidgetVisibility(renderSettings, true); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::Text("Show Framebuffer:");
			ImGui::Separator();

			if (ImGui::MenuItem("Final Render")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetPostProcessingPass()->GetTextureBuffer()); }
			if (ImGui::BeginMenu("Depth Buffers##Menu"))
			{
				if (ImGui::MenuItem("Depth")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetDepthPass()->GetTextureBuffer()); }
				if (ImGui::MenuItem("Postition")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetDepthPass()->GetPositionBuffer()); }
				if (ImGui::MenuItem("Normals")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetDepthPass()->GetNormalBuffer()); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("FlatGeometryLit Buffers"))
			{
				if (ImGui::MenuItem("FlatGeometryLit Color")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetTextureBuffer()); }
				if (ImGui::MenuItem("FlatGeometryLit Normals")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetNormalBuffer()); }
				if (ImGui::MenuItem("FlatGeometryLit Specular")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetSpecularBuffer()); }
				if (ImGui::MenuItem("FlatGeometryLit Roughness")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetRoughnessBuffer()); }
				if (ImGui::MenuItem("FlatGeometryLit UVs")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetFlatGeometryLitPass()->GetUVBuffer()); }
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("SSAO")) { Render::Renderer::Instance()->SetFinalColorBuffer(Render::FrameServer::Instance()->GetSSAOPass()->GetSSAOBuffer()->GetTextureID()); }
			ImGui::Separator();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void MenuBar::ShowFileMenu()
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
		if (ImGui::MenuItem("Save Layout...")) {}
		if (ImGui::MenuItem("Load Layout...")) { }
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
	if (ImGui::MenuItem("Quit", "Alt+F4")) { this->app->Close(); }

}

}
