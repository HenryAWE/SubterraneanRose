/**
 * @file configmenu.cpp
 * @author HenryAWE
 * @brief Configuration panel
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "configmenu.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imguisr.h>
#include "uimgr.hpp"


namespace srose::ui
{
    ConfigPanel::ConfigPanel()
    {
        using std::make_pair;

        constexpr int BUTTON_COUNT = 1;
        m_buttons.reserve(1);
        m_buttons.push_back(make_pair(gettext("srose.ui.configpanel.return") + "###return", &ConfigPanel::Button_Return));
    }

    void ConfigPanel::Update()
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
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>("##configpanel", nullptr, background_flags);
        using std::get;
        for(const auto& i : m_buttons)
        {
            if(ImGui::Button(get<0>(i).c_str()))
            {
                auto cb = get<1>(i);
                if(cb) (this->*cb)();
            }
        }
    }

    void ConfigPanel::Button_Return()
    {
        auto& uimgr = *GetUIManager();
        if(&*uimgr.widget_stack.top() == this)
            uimgr.widget_stack.pop();
    }
} // namespace srose::ui
