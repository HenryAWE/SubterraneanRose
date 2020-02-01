/**
 * @file mainmenu.hpp
 * @author HenryAWE
 * @brief Main Menu
 */

#ifndef SROSE_UI_GUI_mainmenu_hpp_
#define SROSE_UI_GUI_mainmenu_hpp_

#include <vector>
#include <imgui.h>
#include <imguisr.h>
#include "widget.hpp"


namespace srose::ui
{
    class MainMenu : public Widget
    {
    public:
        MainMenu();

        void Update();
    private:
        std::vector<std::string> m_buttons;
    };
} // namespace srose::ui


#endif
