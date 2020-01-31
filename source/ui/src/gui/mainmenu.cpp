/**
 * @file mainmenu.cpp
 * @author HenryAWE
 * @brief Main Menu
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "mainmenu.hpp"
#include <cassert>
#include <imgui_internal.h>


namespace srose::ui
{
    void MainMenu::Update()
    {
        auto& io = ImGui::GetIO();

        int background_flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoSavedSettings;
        ImGui::SetNextWindowPosCenter();
        ImGui::SetNextWindowSize(io.DisplaySize/2.0f);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            "##srose.mainmenu",
            nullptr,
            background_flags
        );
        assert(background);
    }
} // namespace srose::ui
