#include "config.h"
#include "widgetdebugvars.h"
#include "foundation/input/inputmanager.h"
#include "render/render/renderer.h"
#include "IconsFontAwesome5_c.h"

namespace UI
{
DebugVariables::DebugVariables(Display::Window* app) : Widget(app)
{
	this->icon = ICON_FA_BUG;
	this->title = "Debug Variables";
}

void DebugVariables::Update()
{
	Util::String position = "Mouse pos:\n ";
	position.Append(Util::String::FromVec2(Input::InputManager::Instance()->GetMousePosition()));
	ImGui::Text(position.AsCharPtr());

	position = "Relative Mouse pos:\n ";
	position.Append(Util::String::FromVec2(Input::InputManager::Instance()->GetRelativeMousePosition()));
	ImGui::Text(position.AsCharPtr());

	Util::String renderResolution = "Render Resolution:\n";
	renderResolution.AppendInt(Render::Renderer::Instance()->GetRenderResolution().x);
	renderResolution.Append("x");
	renderResolution.AppendInt(Render::Renderer::Instance()->GetRenderResolution().y);
	ImGui::Text(renderResolution.AsCharPtr());

	renderResolution = "Window Resolution:\n";
	renderResolution.AppendInt(Render::Renderer::Instance()->GetWindowResolution().x);
	renderResolution.Append("x");
	renderResolution.AppendInt(Render::Renderer::Instance()->GetWindowResolution().y);
	ImGui::Text(renderResolution.AsCharPtr());
}

}
