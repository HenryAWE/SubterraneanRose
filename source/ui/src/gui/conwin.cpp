/**
 * @file conwin.cpp
 * @author HenryAWE
 * @brief Graphical console
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "conwin.hpp"
#include <iostream>
#include <imgui.h>
#include <imgui_internal.h>
#include <imguisr.h>
#include <sr/ui/uimgr.hpp>


namespace srose::ui
{
    ConsoleWindow::ConsoleWindow(std::string name)
        : Base("", std::move(name))
    {
        Connect(UIManager::GetInstance().OnImbue);

        SetId(gettext("srose.ui.conwin"));
        AddString("logviewer", "srose.ui.conwin.log", "", "###logviewer");
    }

    ConsoleWindow::~ConsoleWindow()
    {

    }

    void ConsoleWindow::Update()
    {
        Base::Update();

        auto& io = ImGui::GetIO();

        auto conwin = BeginContext();
        if(!conwin)
            return;

        constexpr int tabbar_flags = 
             ImGuiTabBarFlags_NoTooltip |
             ImGuiTabBarFlags_Reorderable;
        auto tabbar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabBar>("##tabbar", tabbar_flags);

        UpdateLogViewerTabItem();
    }

    void ConsoleWindow::LoadI18nData()
    {
        Base::LoadI18nData();

        SetId(gettext("srose.ui.conwin"));
    }

    void ConsoleWindow::UpdateLogViewerTabItem()
    {
        auto tabitem = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabItem>(GetString("logviewer").c_str());
        if(!tabitem)
            return;

        if(ImGui::BeginChild("##logs", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar))
        {
            using std::get;
            for(const auto& i : m_logbuf)
            {
                ImVec4 col = {1, 1, 1, 1};
                if(get<0>(i) >= SDL_LOG_PRIORITY_ERROR)
                    col = {1, 0, 0, 1};
                else if(get<0>(i) >= SDL_LOG_PRIORITY_WARN)
                    col = {0.871f, 0.51f, 0.156f, 1.0f};
                ImGui::TextColored(col, get<1>(i).c_str());
                ImGui::Separator();
            }
            if(m_logchanged)
            {
                ImGui::SetScrollHereY();
                m_logchanged = false;
            }
        }
        ImGui::EndChild();
    }
} // namespace srose::ui
