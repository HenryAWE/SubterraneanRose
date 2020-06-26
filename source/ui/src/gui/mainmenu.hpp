/**
 * @file mainmenu.hpp
 * @author HenryAWE
 * @brief Main Menu
 */

#ifndef SROSE_UI_GUI_mainmenu_hpp_
#define SROSE_UI_GUI_mainmenu_hpp_

#include <vector>
#include <utility>
#include <imgui.h>
#include <imguisr.h>
#include <sr/ui/node.hpp>


namespace srose::ui
{
    class MainMenu : public RootNode
    {
    public:
        typedef RootNode Base;

        MainMenu();

        void Update() override;

    private:
        using callback_type = void(MainMenu::*)();
        std::vector<std::pair<std::string, callback_type>> m_buttons;

        std::string m_crash_report_title;
        std::string m_crash_report_info;
        bool m_show_stacktrace = false;
        std::string m_crash_report_stacktrace;

        void LoadI18nData() override;

        void Button_Editor();
        void Button_Config();
        void Button_About();
        void Button_Exit();
    };
} // namespace srose::ui


#endif
