/**
 * @file editor_window.hpp
 * @author HenryAWE
 * @brief Editor window
 */

#ifndef SROSE_UI_GUI_editor_window_hpp_
#define SROSE_UI_GUI_editor_window_hpp_

#include <sr/ui/property.hpp>
#include <imguisr.h>
#include "../widget.hpp"


namespace srose::ui
{
    class EditorWindow : public Widget
    {
        bool m_visible = true;
        std::string window_title;
        ImGuiSR::FileBrowser m_browser;

        struct MenuBarData
        {
            std::string file, open, close;
        }m_menu_bar;
    public:
        EditorWindow();

        void Update();

        void Show() noexcept { m_visible = true; }

    private:
        void UpdateMenuBar();
    };
} // namespace srose::ui


#endif
