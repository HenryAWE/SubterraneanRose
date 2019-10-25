/**
 * @file debug_ui.cpp
 * @author HenryAWE
 * @brief UI for debugging
 */

#include "debug_ui.hpp"
#include <imgui.h>
#include <sr/ui/console/progopt.h>
#include <sr/core/version_info.h>
#include "../i18n/i18n.hpp"


namespace srose::ui
{
    DebugUI::DebugUI(bool* p_open)
        : m_p_open(p_open)
    {
        draw_overlay = (bool)SR_UI_CONSOLE_DrawDebugOverlay();
    }

    void DebugUI::Update()
    {
        if(m_p_open&&*m_p_open == false)
            goto proc_children;
        if(!ImGui::Begin("Debug##SRDBG", m_p_open))
            goto proc_children;

        ImGui::Text(
            "Program Version %d.%d.%d - %s",
            SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH,
            SR_CORE_GitCommitShortID()
        );
        ImGui::Separator();

        ImGui::Checkbox("Draw Overlay", &draw_overlay);
        
        ImGui::End();

    proc_children:
        if(draw_overlay)
            DrawOverlay();
    }

    void DebugUI::DrawOverlay()
    {
        ImGui::SetNextWindowSize(ImVec2(175.0f, 0.0f));
        ImGui::SetNextWindowPos(ImVec2(15.0f, 25.0f));
        ImGui::SetNextWindowBgAlpha(0.35f);
        if(!ImGui::Begin(
            "Debug##SRDBG-OVERLAY",
            nullptr,
            ImGuiWindowFlags_NoFocusOnAppearing|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoNav|
            ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoResize
        )) return;
        ImGui::TextUnformatted("Debug Info");

        if(ImGui::BeginTabBar("##TabBar"))
        {
            if(ImGui::BeginTabItem("i18n"))
            {
                ImGui::BulletText("Translation");

                ImGui::TextWrapped("Default locale: %s (%s)",
                    GetDefaultLanguage()->name().c_str(),
                    GetDefaultLanguage()->iso().c_str()
                );

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
} // namespace srose::ui
