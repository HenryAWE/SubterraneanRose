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
        : Base("srose.editor"), m_srlc_editor(std::make_shared<SrlcEditor>())
    {
        auto& uimgr = UIManager::GetInstance();
        Connect(uimgr.OnImbue);
        m_srlc_editor->Connect(uimgr.OnImbue);

        m_title = gettext("srose.ui.editor");
        m_chkbox_srlc_editor = gettext("srose.ui.srlc-editor") + "##srlc-editor";
        m_button_return = gettext("srose.ui.common.return") + "##return";
    }

    void EditorWindow::Update()
    {
        auto& io = ImGui::GetIO();

        SetFlags(ImGuiWindowFlags_MenuBar);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        auto background = BeginContext();
        ImGui::PopStyleVar();
        if(first_appeared)
        {
            first_appeared = false;
        }

        UpdateMenuBar();
        if(m_show_srlc_editor)
        {
            m_srlc_editor->Open();
            m_srlc_editor->Update();
            m_show_srlc_editor = m_srlc_editor->visible();
        }
    }

    void EditorWindow::LoadI18nData()
    {
        Base::LoadI18nData();

        m_title = gettext("srose.ui.editor");
        m_chkbox_srlc_editor = gettext("srose.ui.srlc-editor") + "##srlc-editor";
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
        ImGui::Checkbox(m_chkbox_srlc_editor.c_str(), &m_show_srlc_editor);
    }

    void EditorWindow::Button_Return()
    {
        auto& uimgr = UIManager::GetInstance();
        uimgr.PopRootNode();
        first_appeared = true;
    }
} // namespace srose::ui
