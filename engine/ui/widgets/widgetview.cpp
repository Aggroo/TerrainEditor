#include "config.h"
#include "widgetview.h"
#include "render/render/renderer.h"
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
	int currWidth = (int)(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	int currHeight = (int)(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

	// Make sure we are pixel perfect
	currWidth -= (currWidth % 2 != 0) ? 1 : 0;
	currHeight -= (currHeight % 2 != 0) ? 1 : 0;

	if (width != currWidth || height != currHeight)
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
