#pragma once
#include "widget.h"

namespace UI
{
class View : public Widget
{
public:

	View(Display::Window* app);

	void Update() override;
};
}