/**
 * @file editor_window.hpp
 * @author HenryAWE
 * @brief Editor window
 */

#ifndef SROSE_UI_GUI_editor_window_hpp_
#define SROSE_UI_GUI_editor_window_hpp_

#include <sr/ui/property.hpp>
#include <imguisr.h>
#include <sr/ui/node.hpp>
#include "srlc_editor.hpp"


namespace srose::ui::editor
{
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
    public:
        typedef RootNode Base;

        EditorWindow();

        void Update() override;

    private:
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
    };
} // namespace srose::ui


#endif
