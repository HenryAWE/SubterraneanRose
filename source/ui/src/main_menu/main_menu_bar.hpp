/**
 * @file main_menu_bar.hpp
 * @author HenryAWE
 * @brief Main menu bar
 */

#ifndef SROSE_UI_MAIN_MENU_main_menu_bar_hpp_
#define SROSE_UI_MAIN_MENU_main_menu_bar_hpp_

#include <string>
#include <sr/core/macros.h>
#include <sr/wm/display.h>
#include "../debug/debug_ui.hpp"


namespace srose::ui::main_menu
{
    struct MainMenuBar
    {
        SR_WM_display* display = nullptr;

        bool show_about = false;
        bool show_imgui_demo = false;
        bool show_debug_ui = false;
        std::string file, quit;
        std::string window, toggle_screen;
        std::string dev;
        std::string help, home_page, about_sr;

        MainMenuBar(SR_WM_display* disp);

        void LoadStrings();

        void Update();

    private:
        void AboutPopup();
    };
} // namespace srose::ui::main_menu


#endif
