#include "config.h"
#include "widgetview.h"
#include "render/render/renderer.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{
View::View(Display::Window* app) : Widget(app)
{
	this->icon = ICON_FA_VIDEO;
	this->title = "View";
	this->flags |= ImGuiWindowFlags_AlwaysAutoResize;
}

void View::Update()
{

	int width = (int)(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	int height = (int)(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

	// Make sure we are pixel perfect
	width -= (width % 2 != 0) ? 1 : 0;
	height -= (height % 2 != 0) ? 1 : 0;
	ImGui::Image((ImTextureID)Render::Renderer::Instance()->GetFinalColorBuffer(), ImVec2(width, height));

	ImGuizmo::SetDrawlist();

	auto win = ImGui::GetCurrentWindowRead();
	ImGuizmo::SetRect(win->Pos.x, win->Pos.y, win->Size.x, win->Size.y);
}

}
