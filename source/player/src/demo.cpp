/**
 * @file demo.cpp
 * @author HenryAWE
 * @brief Player demo
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "demo.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imguisr.h>
#include <sr/wm/winmgr.hpp>


#ifndef SROSE_DISABLE_DEMO

namespace srose::player
{
    PlayerDemoWindow::PlayerDemoWindow()
        : m_stage({350, 430}, *wm::GetRenderer())
    {

    }

    void PlayerDemoWindow::Update()
    {
        m_stage.Update();

        auto& io = ImGui::GetIO();

        constexpr int background_flags = 
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_MenuBar;
        ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_Appearing);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            "Player Demo",
            &open,
            background_flags
        );
        if(!background)
            return;

        auto& screen = m_stage.GetScene().GetScreenTexture();
        if(ImGui::BeginChild("##screen"))
        {
            ImGui::Image(
                screen.GetNativeHandle(),
                screen.GetSizeImVec2(),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }
        ImGui::EndChild();
    }

    void PlayerDemoWindow::Render()
    {
        auto& ren = *wm::GetRenderer();
        m_stage.Render(ren);
    }
} // namespace srose::player

#endif
