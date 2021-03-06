#pragma once
#include "core/refcounted.h"
#include "core/singleton.h"
#include <memory>
#include <vector>

namespace Display
{
	class Window;
}

struct ImFont;

namespace UI
{
class Widget;

class UIServer
{
__DeclareSingleton(UIServer)
public:

	void Setup(Display::Window* window);

	void Update();

	void AddWidget(std::unique_ptr<UI::Widget> widget);
	UI::Widget* GetLastWidget();

	void SetWidgetVisibility(Util::String widgetName, bool visibility);

	ImFont* GetDefaultFont();
	ImFont* GetLargeDefaultFont();

private:

	void ApplyStyle();

	ImFont* defaultFont;
	ImFont* defaultFontLarge;

	Display::Window* window;
	std::vector<std::unique_ptr<UI::Widget>> widgets;
};
}
