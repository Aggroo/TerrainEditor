#pragma once
#include "foundation/util/string.h"
#include "imgui.h"
#include "core/refcounted.h"
#include <functional>

namespace Display 
{
	class Window;
}

namespace UI
{
class Widget
{
public:
	Widget(){}

	Widget(Display::Window* app)
	{
		this->app = app;
	}

	virtual ~Widget() = default;

	bool Begin()
	{
		if (!isWindow || !isVisible)
			return false;

		if (callbackBegin)
			callbackBegin();
			

		if (padding[0] != -1.0f && padding[1] != -1.0f)
		{
			PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
		}

		ImGui::Begin(title.AsCharPtr(), &isVisible, flags);
		styleVarPushes = 0;

		windowBegun = true;

		return true;
	}

	virtual void Update() = 0;

	bool End()
	{
		if (!windowBegun)
			return false;

		ImGui::End();
		ImGui::PopStyleVar(styleVarPushes);
		windowBegun = false;

		return true;
	}

	const Util::String& GetTitle() { return this->title; }

	void RenderTooltip(const char* text)
	{
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(text);
	}

	template<typename T>
	void PushStyleVar(ImGuiStyleVar idx, T val) { ImGui::PushStyleVar(idx, val); styleVarPushes++; }

	void SetVisibility(bool visibility) { isVisible = visibility; }

protected:

	Util::String title;
	bool isVisible = true;
	bool isWindow = true;
	bool windowBegun = false;
	int flags = ImGuiWindowFlags_NoCollapse;
	ImVec2 padding = ImVec2(-1.0f, -1.0f);

	std::function<void()> callbackBegin = nullptr;
	Display::Window* app = nullptr;

private:
	uint8_t styleVarPushes = 0;
};
}
