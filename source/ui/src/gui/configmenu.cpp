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

        constexpr int BUTTON_COUNT = 2;
        m_buttons.reserve(BUTTON_COUNT);
        m_buttons.push_back(make_pair(gettext("srose.ui.configpanel.video") + "###lang", &ConfigPanel::Button_Video));
        m_buttons.push_back(make_pair(gettext("srose.ui.configpanel.lang") + "###lang", &ConfigPanel::Button_Language));
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

        /* Configuration items' list */
        ImVec2 configitems_region(ImGui::GetWindowContentRegionWidth() * 0.18f, io.DisplaySize.y * 0.7f);
        ImGui::BeginChild("##configitems", configitems_region, true, ImGuiWindowFlags_NoScrollbar);
        using std::get;
        const float button_width = ImGui::GetWindowContentRegionWidth();
        for(const auto& i : m_buttons)
        {
            if(ImGui::Button(get<0>(i).c_str(), ImVec2(button_width, 0.0f)))
            {
                auto cb = get<1>(i);
                if(cb) (this->*cb)();
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        /* Item content */
        ImVec2 content_region(ImGui::GetContentRegionAvail().x * 0.85f, io.DisplaySize.y * 0.7f);
        ImGui::BeginChild("##content", content_region, true);
        ImGui::EndChild();
    }

    void ConfigPanel::Button_Video()
    {

    }
    void ConfigPanel::Button_Language()
    {

    }
    void ConfigPanel::Button_Return()
    {
        auto& uimgr = *GetUIManager();
        if(&*uimgr.widget_stack.top() == this)
            uimgr.widget_stack.pop();
    }
} // namespace srose::ui
