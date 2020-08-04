/**
 * @file window.hpp
 * @author HenryAWE
 * @brief Editor window
 */

#ifndef SROSE_UI_EDITOR_window_hpp_
#define SROSE_UI_EDITOR_window_hpp_

#include <sr/ui/property.hpp>
#include <imguisr.h>
#include <sr/ui/node.hpp>
#include <sr/editor/project.hpp>
#include "srlc_editor.hpp"


namespace srose::ui::editor
{
    class EditorState;
    class EditorWindow;

    class FileMenu
    {
        EditorWindow& m_editor;
    public:
        FileMenu(EditorWindow& editor);

        void Update();

        void LoadI18nData(const locale::Language& lang);
    };

    class EditorWindow : public RootNode
    {
        friend class FileMenu;
        bool first_appeared = true;

        std::shared_ptr<srose::editor::Project> m_project;
    public:
        typedef RootNode Base;

        EditorWindow();

        ~EditorWindow();

        void Update() override;

        void NewProject();
        void CloseProject();
        [[nodiscard]]
        bool HasProject() const noexcept { return static_cast<bool>(m_project); }
        [[nodiscard]]
        constexpr const std::shared_ptr<srose::editor::Project>& GetCurrentProject() const noexcept { return m_project; }

        EditorState& GetEditorState();

    private:
        std::unique_ptr<EditorState> m_state;

        static ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_ID> BeginID() noexcept;

        std::string m_title;
        std::string m_chkbox_srlc_editor;
        void LoadI18nData() override;

        void UpdateMenuBar();
        bool m_show_srlc_editor = false;
        std::shared_ptr<SrlcEditor> m_srlc_editor;

        std::string m_button_return;
        void Button_Return();

        FileMenu m_filemenu;
        std::shared_ptr<ImGuiSR::IFileBrowser> m_ifile_dialog;

        void SetWindowTitle();
    };
} // namespace srose::ui


#endif
