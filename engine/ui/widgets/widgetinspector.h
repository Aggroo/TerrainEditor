#pragma once
#include "widget.h"

namespace UI
{
struct LightSettings
{
	Math::vec4 pos;
	Math::vec4 col;
	float radius;
};

class Inspector : public Widget
{
public:
	Inspector(Display::Window* app);

	void Update() override;

private:
	UI::LightSettings light;
};
}
