/**
 * @file imguisr.cpp
 * @author HenryAWE
 * @brief ImGui extensions
 */

#include <imguisr.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>


namespace ImGuiSR
{
    void FileBrowser::Open()
    {
        auto& io = ImGui::GetIO();
        if (!ImGui::IsPopupOpen(m_title.c_str()))
        {
            ImGui::OpenPopup(m_title.c_str());
        }

        constexpr ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_MenuBar;

        bool p_open = true;
        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f);
        ImGui::SetNextWindowPosCenter();
        if (ImGui::BeginPopupModal(m_title.c_str(), &p_open, flags))
        {
            UpdateMainContent();
            ImGui::EndPopup();
        }
    }

    void FileBrowser::UpdateMainContent()
    {

    }
} // namespace ImGuiSR
