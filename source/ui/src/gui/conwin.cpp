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


namespace srose::ui
{
    ConsoleWindow::ConsoleWindow()
    {
        Load();
        RedirectSdlOutput();
    }

    ConsoleWindow::~ConsoleWindow()
    {
        void* user = nullptr;
        SDL_LogGetOutputFunction(nullptr, &user);
        if(user == this)
        {
            SDL_LogSetOutputFunction(nullptr, nullptr);
        }
    }

    void ConsoleWindow::Update()
    {
        Widget::Update();

        auto& io = ImGui::GetIO();

        constexpr int  conwin_flags = 
            ImGuiWindowFlags_NoSavedSettings;
        ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(io.DisplaySize*0.8f, ImGuiCond_Appearing);
        auto conwin = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            m_title.c_str(),
            &open,
            conwin_flags
        );
        if(!conwin)
            return;

        constexpr int tabbar_flags = 
             ImGuiTabBarFlags_NoTooltip |
             ImGuiTabBarFlags_Reorderable;
        auto tabbar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabBar>("##tabbar", tabbar_flags);

        UpdateLogViewerTabItem();
    }

    void ConsoleWindow::Load()
    {
        m_title = gettext("srose.ui.conwin") + "###conwin";
        m_logviewer = gettext("srose.ui.conwin.log");
    }

    void ConsoleWindow::UpdateLogViewerTabItem()
    {
        auto tabitem = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabItem>(m_logviewer.c_str());
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

    void ConsoleWindow::RedirectSdlOutput()
    {
        SDL_LogSetOutputFunction(
            [](void* this_, int category, SDL_LogPriority priority, const char* message)
            {
                static_cast<ConsoleWindow*>(this_)->SdlLogCallback(
                    category,
                    priority,
                    message
                );
            },
            this
        );
    }
    void ConsoleWindow::SdlLogCallback(int category, SDL_LogPriority priority, const char* message)
    {
#ifdef NDEBUG
        if(verbose || priority >= SDL_LOG_PRIORITY_WARN)
#else
        if(verbose || priority >= SDL_LOG_PRIORITY_DEBUG)
#endif
        {
            std::cout << message << std::endl;
        }
        m_logbuf.push_back({priority, message});
        m_logchanged = true;
    }
} // namespace srose::ui
