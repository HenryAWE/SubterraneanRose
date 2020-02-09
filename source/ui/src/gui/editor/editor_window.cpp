/**
 * @file editor_window.cpp
 * @author HenryAWE
 * @brief Editor window
 */

#include "editor_window.hpp"
#include <cassert>
#include <imgui.h>
#include <imguisr.h>
#include <sr/ui/gui/uimgr.hpp>


namespace srose::ui::editor
{
    EditorWindow::EditorWindow()
    {
        LoadAll();
    }

    void EditorWindow::Update()
    {
        auto& io = ImGui::GetIO();

        constexpr int background_flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_MenuBar;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::SetNextWindowPosCenter();
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>("##srose.ui.editor", nullptr, background_flags);
        assert(background);
        ImGui::PopStyleVar();

        UpdateMenuBar();
    }

    void EditorWindow::LoadAll()
    {
        m_title = gettext("srose.ui.editor");
        m_button_return = gettext("srose.ui.common.return") + "##return";
    }

    void EditorWindow::UpdateMenuBar()
    {
        auto bar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_MenuBar>();
        if(ImGui::MenuItem(m_button_return.c_str()))
            Button_Return();
        ImGui::Separator();
        if (!bar)
            return;
        ImGui::Text(m_title.c_str());
        ImGui::Separator();
    }

    void EditorWindow::Button_Return()
    {
        auto& uimgr = *GetUIManager();
        if(&*uimgr.widget_stack.top() == this)
            uimgr.widget_stack.pop();
    }
} // namespace srose::ui
