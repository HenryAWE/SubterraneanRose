/**
 * @file editor_window.cpp
 * @author HenryAWE
 * @brief Editor window
 */

#include "editor_window.hpp"
#include <cassert>
#include <imgui.h>
#include <imguisr.h>


namespace srose::ui::editor
{
    EditorWindow::EditorWindow()
    {
        LoadAll();
    }

    void EditorWindow::Update()
    {
        constexpr int background_flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_MenuBar;
        
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(m_title.c_str(), nullptr, background_flags);
        assert(background);

        UpdateMenuBar();
    }

    void EditorWindow::LoadAll()
    {
        m_title = gettext("srose.ui.editor");
    }

    void EditorWindow::UpdateMenuBar()
    {
        auto bar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_MenuBar>();
        if (!bar)
            return;
        ImGui::Text(m_title.c_str());
        ImGui::Separator();
    }
} // namespace srose::ui
