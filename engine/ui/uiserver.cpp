#include "config.h"
#include "uiserver.h"
#include "ui/widgets/widgettoolbar.h"
#include "ui/widgets/widgetinspector.h"
#include "ui/widgets/widgetmenubar.h"
#include "ui/widgets/widgetperlingenerator.h"
#include "ui/widgets/widgetterrainsettings.h"
#include "ui/widgets/widgetview.h"
#include "ImGuizmo.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{
UIServer::UIServer()
{
	
}

void UIServer::Setup(Display::Window* window)
{
	this->window = window;

	ImGuiIO& io = ImGui::GetIO();

	// merge in icons from Font Awesome
	ImFontConfig cfg;
	cfg.MergeMode = true;
	cfg.GlyphMinAdvanceX = 14.0f;
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	io.Fonts->AddFontFromFileTTF("resources/fonts/fa-solid-900.ttf", 16.0f, &cfg, icon_ranges);
	defaultFont = io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Medium.ttf", 16.0f);
	defaultFontLarge = io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Medium.ttf", 18.0f);

	ApplyStyle();
}

void UIServer::Update()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	//ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y + 35, viewport->Size.x, viewport->Size.y - 35);

	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 35));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - 35));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking;
	ImGui::Begin("Base", NULL, window_flags);
	
	ImGuiID dockSpace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockSpace_id);
	ImGui::SetNextWindowDockID(dockSpace_id, ImGuiCond_Once);

	for (auto& widget : widgets)
	{
		widget->Begin();
		widget->Update();
		widget->End();
	}

	ImGui::End();
}

void UIServer::AddWidget(std::unique_ptr<UI::Widget> widget)
{
	this->widgets.emplace_back(std::move(widget));
}

UI::Widget* UIServer::GetLastWidget()
{
	return this->widgets.back().get();
}

void UIServer::SetWidgetVisibility(Util::String widgetName, bool visibility)
{
	for (auto& widget : widgets)
	{
		if (widget.get()->GetTitle().HashCode() == widgetName.HashCode())
		{
			widget.get()->SetVisibility(visibility);
		}
	}
}

ImFont* UIServer::GetDefaultFont()
{
	return this->defaultFont;
}

ImFont* UIServer::GetLargeDefaultFont()
{
	return this->defaultFontLarge;
}

void UIServer::ApplyStyle()
{
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	float fontSize = 16.0f;
	float roundness = 2.0f;
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 text = ImVec4(0.76f, 0.77f, 0.8f, 1.0f);
	ImVec4 black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 backgroundVeryDark = ImVec4(0.08f, 0.086f, 0.094f, 1.00f);
	ImVec4 backgroundDark = ImVec4(0.141f, 0.141f, 0.141f, 1.00f);
	ImVec4 backgroundMedium = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	ImVec4 backgroundLight = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	ImVec4 backgroundLighter = ImVec4(0.37f, 0.38f, 0.39f, 1.0f);
	ImVec4 highlightBlue = ImVec4(0.345f, 0.345f, 0.345f, 1.0f);
	ImVec4 highlightBlueActive = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	ImVec4 highlightBlueHovered = ImVec4(0.345f, 0.345f, 0.345f, 1.0f);
	ImVec4 barBackground = ImVec4(0.078f, 0.082f, 0.09f, 1.0f);
	ImVec4 bar = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	ImVec4 barlighter = ImVec4(0.23f, 0.23f, 0.23f, 1.0f);
	ImVec4 barHovered = ImVec4(0.411f, 0.411f, 0.411f, 1.0f);
	ImVec4 barActive = ImVec4(0.345f, 0.345f, 0.345f, 1.0f);

	// Spatial
	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	//style.WindowMinSize		= ImVec2(160, 20);
	style.FramePadding = ImVec2(5, 5);
	style.ItemSpacing = ImVec2(6, 5);
	//style.ItemInnerSpacing	= ImVec2(6, 4);
	style.Alpha = 1.0f;
	style.WindowRounding = roundness;
	style.FrameRounding = roundness+1.0f;
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
	style.Colors[ImGuiCol_Border] = bar;
	//style.Colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_FrameBgHovered] = backgroundDark;
	style.Colors[ImGuiCol_FrameBgActive] = backgroundMedium;
	style.Colors[ImGuiCol_TitleBg] = backgroundVeryDark;
	//style.Colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = backgroundVeryDark;
	style.Colors[ImGuiCol_MenuBarBg] = backgroundVeryDark;
	style.Colors[ImGuiCol_ScrollbarBg] = barBackground;
	style.Colors[ImGuiCol_ScrollbarGrab] = bar;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = barHovered;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = barActive;
	style.Colors[ImGuiCol_CheckMark] = white;
	style.Colors[ImGuiCol_SliderGrab] = bar;
	style.Colors[ImGuiCol_SliderGrabActive] = barActive;
	style.Colors[ImGuiCol_Button] = bar;
	style.Colors[ImGuiCol_ButtonHovered] = backgroundMedium;
	style.Colors[ImGuiCol_ButtonActive] = backgroundDark;
	style.Colors[ImGuiCol_Header] = backgroundMedium; // selected items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderHovered] = bar; // hovered items (tree, menu bar etc.)
	style.Colors[ImGuiCol_HeaderActive] = backgroundMedium;
	style.Colors[ImGuiCol_Separator] = backgroundLighter;
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
	style.Colors[ImGuiCol_DragDropTarget] = backgroundLighter;
	style.Colors[ImGuiCol_Tab] = backgroundDark;
	style.Colors[ImGuiCol_TabHovered] = barlighter; 
	style.Colors[ImGuiCol_TabActive] = backgroundMedium;
	style.Colors[ImGuiCol_TabUnfocused] = bar; 
	style.Colors[ImGuiCol_TabUnfocusedActive] = backgroundMedium;
	style.Colors[ImGuiCol_NavHighlight] = bar;
	style.Colors[ImGuiCol_NavWindowingHighlight] = bar;
	style.Colors[ImGuiCol_NavWindowingDimBg] = bar;
	style.Colors[ImGuiCol_ModalWindowDimBg] = bar;

	//style.Colors[ImGuiCol_ModalWindowDarkening]	= ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
}
}
