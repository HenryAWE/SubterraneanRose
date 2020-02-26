/**
 * @file demo.cpp
 * @author HenryAWE
 * @brief Audio demo
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "demo.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imguisr.h>
#include <SDL_mixer.h>


namespace srose::audio
{
    AudioDemoWindow::AudioDemoWindow()
    {
        
    }

    AudioDemoWindow::~AudioDemoWindow()
    {

    }

    void AudioDemoWindow::Update()
    {
        auto& io = ImGui::GetIO();

        constexpr int background_flags = 
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_MenuBar;
        ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_Appearing);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            "Audio Demo",
            &open,
            background_flags
        );
        if(!background)
            return;

        UpdateMenuBar();

        bool any_tab =
            m_show_information;
        if(!any_tab)
        {
            ImGui::TextDisabled("(Empty)");
            return;
        }
        else
        {
            UpdateTabBar();
        }
    }

    void AudioDemoWindow::UpdateMenuBar()
    {
        auto menubar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_MenuBar>();
        if(menubar)
        {
            auto tabs = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Menu>("Tabs");
            if(tabs)
            {
                ImGui::Checkbox("Information", &m_show_information);
            }
        }
    }
    void AudioDemoWindow::UpdateTabBar()
    {
        constexpr int tabbar_flags = 
            ImGuiTabBarFlags_NoTooltip |
            ImGuiTabBarFlags_AutoSelectNewTabs |
            ImGuiTabBarFlags_Reorderable;
        auto tabbar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabBar>("##tabbar", tabbar_flags);
        if(tabbar)
        {
            if(m_show_information) InformationTabItem();
        }
    }

    void AudioDemoWindow::InformationTabItem()
    {
        auto tabitem =  ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabItem>(
            "Information",
            &m_show_information
        );
        if(!tabitem)
            return;

        SDL_version mixer = {};
        SDL_MIXER_VERSION(&mixer);
        const SDL_version* mixer_runtime = Mix_Linked_Version();
        ImGui::BulletText(
            "SDL_mixer version\n"
            "Compile-time: %d.%d.%d\tRuntime: %d.%d.%d",
            mixer.major, mixer.minor, mixer.patch,
            mixer_runtime->major, mixer_runtime->minor, mixer_runtime->patch
        );

        ImGui::BulletText(
            "Audio driver\n"
            "Current: %s",
            SDL_GetCurrentAudioDriver()
        );
    }
} // namespace srose::audio
