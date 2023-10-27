#pragma once
#include "widget.h"
#include <ui/commands/commandmanager.h>

namespace UI
{
class MenuBar : public Widget
{

public:
	explicit MenuBar(Display::Window* app);
	void Update() override;

private:
	void ShowFileMenu();
	
};
}
