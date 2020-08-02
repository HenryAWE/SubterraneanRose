/**
 * @file editor_window.cpp
 * @author HenryAWE
 * @brief Editor window
 */

#include "editor_window.hpp"
#include <cassert>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/termination.hpp>
#include <imgui.h>
#include <imguisr.h>
#include <sr/ui/uimgr.hpp>
#include <sr/trace/log.hpp>


namespace srose::ui::editor
{
    namespace sc = boost::statechart;
    namespace mpl = boost::mpl;

    class EventUpdate;
    class EventQuit;
    class StateIdle;
    class EditorState : public sc::state_machine<EditorState, StateIdle>
    {
    public:
        EditorState(EditorWindow& editor_) : editor(editor_) {}

        EditorWindow& editor;
    };

    class StateIdle : public sc::simple_state<StateIdle, EditorState>
    {
    public:
        typedef mpl::list<
            sc::custom_reaction<EventUpdate>,
            sc::termination<EventQuit>
        > reactions;

        StateIdle()
        {
            BOOST_LOG_TRIVIAL(info) << "[UI] Enter StateIdle";
        }

        ~StateIdle()
        {
            BOOST_LOG_TRIVIAL(info) << "[UI] Leave StateIdle";
        }

        sc::result react(const EventUpdate&)
        {
            return forward_event();
        }
    };

    class EventUpdate : public sc::event<EventUpdate> {};
    class EventQuit : public sc::event<EventQuit> {};


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
        if(ImGui::MenuItem("New"))
        {
            m_editor.NewProject();
        }
        if(m_editor.HasProject())
        {
            ImGui::Separator();
            // TODO: Implement these menu items
            if(ImGui::MenuItem("Save")) {}
            if(ImGui::MenuItem("Save As")) {}
            ImGui::Separator();
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
        if(m_show_srlc_editor)
        {
            m_srlc_editor->Open();
            m_srlc_editor->Update();
            m_show_srlc_editor = m_srlc_editor->visible();
        }
        m_ifile_dialog->Update();
        m_state->process_event(EventUpdate{});

        if(m_state->terminated())
        {
            CloseProject();
            UIManager::GetInstance().PopRootNode();
            first_appeared = true;
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

    ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_ID> EditorWindow::BeginID() noexcept
    {
        return ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_ID>("srose.editor");
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
