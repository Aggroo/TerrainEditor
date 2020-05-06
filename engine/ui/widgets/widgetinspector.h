#pragma once
#include "widget.h"
#include "ImGuizmo.h"

namespace UI
{
struct LightSettings
{
	Math::vec4 pos;
	Math::vec4 col;
	float intensity;
	float radius;
};

class Inspector : public Widget
{
	struct transformGizmo
	{
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::MODE::WORLD;
		bool useSnap = false;
		float snap[3] = { 1.f, 1.f, 1.f };
	} gizmo;

public:
	Inspector(Display::Window* app);

	void Update() override;

private:
	UI::LightSettings light;
};
}
