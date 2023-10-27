#include "config.h"
#include "widgetdebugvars.h"
#include "foundation/input/inputmanager.h"
#include "render/render/renderer.h"
#include "render/server/pickingServer.h"
#include "render/server/FrameServer.h"
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

	Math::vec2 mousePos = Input::InputManager::Instance()->GetRelativeMousePosition();
	position = "Relative Mouse pos:\n ";
	position.Append(Util::String::FromVec2(mousePos));
	ImGui::Text(position.AsCharPtr());
	
	if (Input::InputManager::Instance()->GetButtonMouse(Input::MouseButton::LEFT))
	{
		Util::String worldSpace = "World Space Pos:\n ";
		worldSpace.Append(Util::String::FromVec4(Render::PickingServer::Instance()->GetWorldSpacePosition(mousePos.x(), mousePos.y(), Render::FrameServer::Instance()->GetDepthPass()->GetFrameBufferObject())));
		ImGui::Text(worldSpace.AsCharPtr());
	}
	

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
