#pragma once
#include "widget.h"

namespace UI
{
class MenuBar : public Widget
{

public:
	MenuBar(Display::Window* app);
	void Update() override;

private:
	void ShowFileMenu();
};
}
