#pragma once
#include "widget.h"

namespace UI
{
class RenderSettings : public Widget
{
public:

	RenderSettings(Display::Window* app);

	void Update() override;

	void ModalWindows();

private:

	bool envTexPopup;
};
}