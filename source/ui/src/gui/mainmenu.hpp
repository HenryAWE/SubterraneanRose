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
#include <sr/ui/gui/widget.hpp>


namespace srose::ui
{
    class MainMenu : public Widget
    {
    public:
        MainMenu();

        void Update() override;

    private:
        using callback_type = void(MainMenu::*)();
        std::vector<std::pair<std::string, callback_type>> m_buttons;

        void Button_Editor();
        void Button_Config();
        void Button_About();
        void Button_Exit();
    };
} // namespace srose::ui


#endif
