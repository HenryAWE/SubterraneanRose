/**
 * @file about.cpp
 * @author HenryAWE
 * @brief About page
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "about.hpp"
#include <cassert>
#include <imgui.h>
#include <imgui_internal.h>
#include <sr/ui/uimgr.hpp>
#include <sr/util/open_in_browser.hpp>


namespace srose::ui
{
    About::About()
    {
        Load();
    }

    void About::Update()
    {
        Widget::Update();

        auto& io = ImGui::GetIO();

        constexpr int background_flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings;
        ImGui::SetNextWindowSize(io.DisplaySize);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>("##about", nullptr, background_flags);
        assert(background);

        if(ImGui::Button(GetString("return").c_str()))
        {
            auto& uimgr = UIManager::GetInstance();
            if(&*uimgr.widget_stack.top() == this)
                uimgr.widget_stack.pop();
        }
        if(ImGui::Button(GetString("website").c_str()))
        {
            util::OpenInBrowser("https://github.com/HenryAWE/SubterraneanRose");
        }
    }

    void About::Load()
    {
        AddString("return", "srose.ui.common.return");
        AddString("website", "srose.ui.about.website");
    }
} // namespace srose::ui
