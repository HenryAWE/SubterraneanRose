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
    FileMenu::FileMenu(EditorWindow& editor)
        : m_editor(editor) {}

    void FileMenu::Update()
    {
        auto menu = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Menu>("File");
        if(!menu)
            return;
        if(ImGui::MenuItem("Open"))
        {
            m_editor.m_ifile_dialog->SetTitle("Open Project");
            m_editor.m_ifile_dialog->Show();
        }
        if(ImGui::MenuItem("Exit"))
        {
            m_editor.Button_Return();
        }
    }

    void FileMenu::LoadI18nData(const locale::Language& lang)
    {

    }


    EditorWindow::EditorWindow()
        : Base("srose.editor"),
        m_srlc_editor(std::make_shared<SrlcEditor>()),
        m_filemenu(*this),
        m_ifile_dialog(ImGuiSR::CreateIFileBrowser(false))
    {
        auto& uimgr = UIManager::GetInstance();
        Connect(uimgr.OnImbue);
        m_srlc_editor->Connect(uimgr.OnImbue);
        m_filemenu.LoadI18nData(*getptr());

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
        m_ifile_dialog->Update();
    }

    void EditorWindow::LoadI18nData()
    {
        Base::LoadI18nData();

        m_title = gettext("srose.ui.editor");
        m_chkbox_srlc_editor = gettext("srose.ui.srlc-editor") + "##srlc-editor";

        m_filemenu.LoadI18nData(*getptr());
    }

    void EditorWindow::UpdateMenuBar()
    {
        auto bar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_MenuBar>();
        if (!bar)
            return;
        ImGui::Text(m_title.c_str());
        ImGui::Separator();
        ImGui::Checkbox(m_chkbox_srlc_editor.c_str(), &m_show_srlc_editor);
        m_filemenu.Update();
    }

    void EditorWindow::Button_Return()
    {
        auto& uimgr = UIManager::GetInstance();
        uimgr.PopRootNode();
        first_appeared = true;
    }
} // namespace srose::ui
