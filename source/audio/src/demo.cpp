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
        LoadDevicesList();
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

    void AudioDemoWindow::LoadDevicesList()
    {
        int count = SDL_GetNumAudioDevices(false);
        if(count < 0)
        {
            m_devices_list.clear();
            return;
        }
        m_devices_list.resize(count);
        for(int i = 0; i < count; ++i)
        {
            m_devices_list[i] = SDL_GetAudioDeviceName(i, false);
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

        ImGui::BulletText("%d audio device(s)", (int)m_devices_list.size());
        ImGui::SameLine();
        if(ImGui::Button("Refresh"))
        {
            LoadDevicesList();
        }
        if(ImGui::BeginChild("##devices", ImVec2(0, 0), true))
        {
            if(m_devices_list.empty())
            {
                ImGui::TextDisabled("(Audio device not existed)");
            }
            else
            {
                for(int i = 0; i < m_devices_list.size(); ++i)
                {
                    ImGui::Text("%d\t%s", i, m_devices_list[i].c_str());
                }
            }
        }
        ImGui::EndChild();
    }
} // namespace srose::audio
