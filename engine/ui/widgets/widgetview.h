#pragma once
#include "widget.h"

namespace UI
{
class View : public Widget
{
public:

	explicit View(Display::Window* app);

	void Update() override;
	void UpdateRenderResolution(int width, int height);

private:
	int width, height;
};
}