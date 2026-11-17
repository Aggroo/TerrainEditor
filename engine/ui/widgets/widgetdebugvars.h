#pragma once
#include "widget.h"
#include "imgui.h"
#include "ImGuizmo.h"

namespace UI
{

class DebugVariables : public Widget
{


public:
	explicit DebugVariables(Display::Window* app);

	void Update() override;

private:

};
}
