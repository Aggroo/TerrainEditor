#pragma once
#include "imgui.h"
#include <fstream>
#include <string>

namespace ImGui
{


void ShutdownDock();
void RootDock(const ImVec2& pos, const ImVec2& size);
bool BeginDock(const char* label, bool* opened = nullptr, ImGuiWindowFlags extra_flags = 0);
//DockContext& GetDock(const char* label, bool* opened = nullptr);
void EndDock();
void SetDockActive();
void SaveDock(const std::string& filename);
void LoadDock(const std::string& filename = "layout/default.layout");


} // namespace ImGui