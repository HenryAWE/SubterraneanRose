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
    }
} // namespace srose::audio
