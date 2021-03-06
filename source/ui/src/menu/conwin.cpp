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
#include <sr/trace/log.hpp>


namespace srose::ui
{
    ConsoleWindow::ConsoleWindow(std::string name)
        : Base("", std::move(name)), m_backend(std::make_shared<std::stringstream>())
    {
        trace::AddStream(m_backend);

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
        if(visible() == false)
        {
            auto& nodes = UIManager::GetInstance().GetStandaloneNodes();
            auto iter = std::find_if(
                nodes.begin(), nodes.end(),
                [this](const auto& v) { return v.get() == this; }
            );
            nodes.erase(iter);
        }
        if(!conwin)
            return;

        for(std::string line; std::getline(*m_backend, line);)
        {
            m_logbuf.push_back(std::tuple(std::move(line)));
            m_logchanged = true;
        }

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
                auto* text = get<0>(i).c_str();
                ImGui::TextUnformatted(text, text + get<0>(i).length());
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
