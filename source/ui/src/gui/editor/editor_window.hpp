/**
 * @file editor_window.hpp
 * @author HenryAWE
 * @brief Editor window
 */

#ifndef SROSE_UI_GUI_editor_window_hpp_
#define SROSE_UI_GUI_editor_window_hpp_

#include <boost/noncopyable.hpp>
#include <sr/ui/property.hpp>


namespace srose::ui
{
    class EditorWindow : boost::noncopyable
    {
        bool m_visible = true;
        std::string window_title;

        struct MenuBarData
        {
            std::string file, open, close;
        }m_menu_bar;
    public:
        EditorWindow();

        void Update();

        void Show() noexcept;

    private:
        void UpdateMenuBar();
    };
} // namespace srose::ui


#endif
