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
    }
} // namespace srose::ui
