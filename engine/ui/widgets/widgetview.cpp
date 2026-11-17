#include "config.h"
#include "widgetview.h"
#include "render/render/renderer.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "IconsFontAwesome5_c.h"
#include "foundation/input/inputmanager.h"

namespace UI
{
View::View(Display::Window* app) : Widget(app)
{
	this->icon = ICON_FA_VIDEO;
	this->title = "View";
	this->flags |= ImGuiWindowFlags_AlwaysAutoResize;
	const Render::Resolution& res = Render::Renderer::Instance()->GetRenderResolution();
	width = res.x;
	height = res.y;
}

void View::Update()
{
	int currWidth = (int)ImGui::GetContentRegionAvail().x;
	int currHeight = (int)ImGui::GetContentRegionAvail().y;

	// Make sure we are pixel perfect
	currWidth -= (currWidth % 2 != 0) ? 1 : 0;
	currHeight -= (currHeight % 2 != 0) ? 1 : 0;

	// Skip resizing while the view has no content area (collapsed, hidden or not
	// laid out yet), otherwise the renderer would get a 0x0 resolution.
	const bool hasArea = currWidth > 0 && currHeight > 0;
	if (hasArea && (width != currWidth || height != currHeight))
		UpdateRenderResolution(currWidth, currHeight);

	ImVec2 test = {ImGui::GetMousePos().x - ImGui::GetCursorScreenPos().x, ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y};
	ImGui::Image((ImTextureID)Render::Renderer::Instance()->GetFinalColorBuffer(), ImVec2(width, height));

	ImGuizmo::SetDrawlist();

	auto win = ImGui::GetCurrentWindowRead();
	ImGuizmo::SetRect(win->Pos.x, win->Pos.y, win->Size.x, win->Size.y);
	Input::InputManager::Instance()->UpdateRelativeMousePosition(test.x, test.y);
}

void View::UpdateRenderResolution(int width, int height)
{
	Render::Renderer::Instance()->SetRenderResolution(width, height);
	const Render::Resolution& res = Render::Renderer::Instance()->GetRenderResolution();
	this->width = res.x;
	this->height = res.y;
}

}
