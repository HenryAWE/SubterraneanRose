/**
 * @file editor_window.cpp
 * @author HenryAWE
 * @brief Editor window
 */

#include "editor_window.hpp"
#include <imgui.h>
#include <imguisr.h>
#include <sr/locale/locale.hpp>


namespace srose::ui
{
    EditorWindow::EditorWindow()
        : window_title(gettext("srose.ui.editor") + "###srose.ui.EditorWindow")
    {
        m_menu_bar.file = gettext("srose.ui.file");
        m_menu_bar.open = gettext("srose.ui.open");
        m_menu_bar.close = gettext("srose.ui.close");
    }

    void EditorWindow::Update()
    {
        if (!m_visible)
            return;

        constexpr ImGuiWindowFlags editor_flags =
            ImGuiWindowFlags_MenuBar;

        auto id = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_ID>("srose.ui.EditorWindow");
        
        auto window = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(window_title.c_str(), &m_visible, editor_flags);
        if (window)
        {
            UpdateMenuBar();
        }
    }

    void EditorWindow::UpdateMenuBar()
    {
        auto bar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_MenuBar>();
        if (!bar)
            return;

        if (auto menu = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Menu>(m_menu_bar.file.c_str()))
        {
            if (ImGui::MenuItem(m_menu_bar.close.c_str(), "Ctrl+Q")) { m_visible = false; }
        }
    }
} // namespace srose::ui
