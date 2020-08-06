/**
 * @file window.cpp
 * @author HenryAWE
 * @brief Editor window
 */

#include "window.hpp"
#include <cassert>
#include <boost/archive/text_iarchive.hpp>
#include <imgui.h>
#include <imguisr.h>
#include <sr/ui/uimgr.hpp>
#include <sr/trace/log.hpp>
#include "impl.hpp"
#include "state.hpp"


namespace srose::ui::editor
{
    FileMenu::FileMenu(EditorWindow& editor)
        : m_editor(editor) {}

    void FileMenu::Update()
    {
        auto menu = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Menu>("File");
        if(!menu)
            return;
        if(m_editor.HasProject())
        {
            if(ImGui::MenuItem("Close"))
            {
                m_editor.CloseProject();
            }
        }
        ImGui::Separator();
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
        m_filemenu(*this)
    {
        auto& uimgr = UIManager::GetInstance();
        Connect(uimgr.OnImbue);
        m_filemenu.LoadI18nData(*getptr());

        m_title = gettext("srose.ui.editor");
        m_button_return = gettext("srose.ui.common.return") + "##return";

        m_impl = std::make_unique<EditorImpl>();
        m_state = std::make_unique<EditorState>(*this);
    }

    EditorWindow::~EditorWindow() = default;

    void EditorWindow::Update()
    {
        auto id = BeginID();
        auto& io = ImGui::GetIO();

        SetFlags(ImGuiWindowFlags_MenuBar);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        auto background = BeginContext();
        ImGui::PopStyleVar();
        if(first_appeared)
        {
            first_appeared = false;
            m_state->initiate();
        }

        UpdateMenuBar();

        m_state->process_event(EventUpdate{});
        m_impl->Update();

        if(m_state->terminated())
        {
            CloseProject();
            UIManager::GetInstance().PopRootNode();
            first_appeared = true;
        }
    }

    void EditorWindow::OpenProject(std::istream& is)
    {
        try
        {
            auto tmp = std::make_shared<srose::editor::Project>();
            boost::archive::text_iarchive iar(is);
            iar >> *tmp;
            m_project.swap(tmp);
            SetWindowTitle();
        }
        catch(...)
        {
            BOOST_LOG_TRIVIAL(error) << "[UI.Editor] Load failed";
        }
    }
    void EditorWindow::NewProject()
    {
        m_project = std::make_shared<srose::editor::Project>();
        SetWindowTitle();
    }
    void EditorWindow::CloseProject()
    {
        m_project.reset();
        SetWindowTitle();
    }

    EditorImpl& EditorWindow::GetEditorImpl()
    {
        return *m_impl;
    }
    EditorState& EditorWindow::GetEditorState()
    {
        assert(m_state != nullptr);
        return *m_state;
    }

    ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_ID> EditorWindow::BeginID() noexcept
    {
        return ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_ID>("srose.editor");
    }

    void EditorWindow::LoadI18nData()
    {
        Base::LoadI18nData();

        m_title = gettext("srose.ui.editor");

        m_filemenu.LoadI18nData(*getptr());
    }

    void EditorWindow::UpdateMenuBar()
    {
        auto bar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_MenuBar>();
        if (!bar)
            return;
        ImGui::Text(m_title.c_str());
        ImGui::Separator();
        m_filemenu.Update();
    }

    void EditorWindow::Button_Return()
    {
        m_state->process_event(EventQuit{});
    }

    void EditorWindow::SetWindowTitle()
    {
        auto& win = UIManager::GetInstance().GetWindow();
        if(HasProject())
        {
            win.SetTitle("Subterranean Rose - " + m_project->name());
        }
        else
        {
            win.SetTitle("Subterranean Rose");
        }
    }
} // namespace srose::ui
