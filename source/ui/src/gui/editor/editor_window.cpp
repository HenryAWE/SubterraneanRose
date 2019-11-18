/**
 * @file editor_window.cpp
 * @author HenryAWE
 * @brief Editor window
 */

#include "editor_window.hpp"
#include <imgui.h>
#include <sr/locale/locale.hpp>


namespace srose::ui
{
    EditorWindow::EditorWindow()
        : window_title(SRTR("srose.ui.editor").str() + "###srose.ui.EditorWindow")
    {
        m_menu_bar.file = SRTR("srose.ui.file");
        m_menu_bar.open = SRTR("srose.ui.open");
        m_menu_bar.close = SRTR("srose.ui.close");
    }

    void EditorWindow::Update()
    {
        if (!m_visible)
            return;

        constexpr ImGuiWindowFlags editor_flags =
            ImGuiWindowFlags_MenuBar;

        ImGui::PushID("srose.ui.EditorWindow");
        if (ImGui::Begin(window_title.c_str(), &m_visible, editor_flags))
        {
            UpdateMenuBar();
        }
        ImGui::End();
        ImGui::PopID();
    }

    void EditorWindow::UpdateMenuBar()
    {
        if (!ImGui::BeginMenuBar())
            return;

        if (ImGui::BeginMenu(m_menu_bar.file.c_str()))
        {
            if (ImGui::MenuItem(m_menu_bar.close.c_str(), "Ctrl+Q")) { m_visible = false; }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
} // namespace srose::ui
