#pragma once
#include "widget.h"
#include "ImGuizmo.h"

namespace UI
{

class DebugVariables : public Widget
{


public:
	DebugVariables(Display::Window* app);

	void Update() override;

private:

};
}
