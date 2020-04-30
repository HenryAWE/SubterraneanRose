/**
 * @file editor_window.cpp
 * @author HenryAWE
 * @brief Editor window
 */

#include "editor_window.hpp"
#include <cassert>
#include <imgui.h>
#include <imguisr.h>
#include <sr/ui/uimgr.hpp>


namespace srose::ui::editor
{
    EditorWindow::EditorWindow()
        : m_srlc_editor(std::make_shared<SrlcEditor>())
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
        if(first_appeared)
        {
            SetWindowSubtitle();
            first_appeared = false;
        }

        UpdateMenuBar();
        if(m_show_srlc_editor)
        {
            m_srlc_editor->open = true;
            m_srlc_editor->Update();
            m_show_srlc_editor = m_srlc_editor->open;
        }
    }

    void EditorWindow::LoadAll()
    {
        m_title = gettext("srose.ui.editor");
        m_chkbox_srlc_editor = gettext("srose.ui.srlc-editor") + "##srlc-editor";
        m_button_return = gettext("srose.ui.common.return") + "##return";
    }
    void EditorWindow::OnImbue()
    {
        LoadAll();
        m_srlc_editor->imbue(getloc());
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
        ImGui::Checkbox(m_chkbox_srlc_editor.c_str(), &m_show_srlc_editor);
    }

    void EditorWindow::Button_Return()
    {
        auto& uimgr = *GetUIManager();
        if(&*uimgr.widget_stack.top() == this)
        {
            uimgr.widget_stack.pop();
            if(!uimgr.widget_stack.empty())
                uimgr.widget_stack.top()->SetWindowSubtitle();
        }
        first_appeared = true;
    }
} // namespace srose::ui
