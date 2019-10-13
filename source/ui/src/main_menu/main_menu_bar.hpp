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


namespace srose::ui::main_menu
{
    struct MainMenuBarContext
    {
        bool show_about = false;
        bool show_imgui_demo = false;
        std::string file, quit;
        std::string window, toggle_screen;
        std::string dev;
        std::string help, home_page, about_sr;
    };

    void SRSCALL LoadStrings(MainMenuBarContext* ctx);

    /**
     * @brief Main menu
     * 
     * @param ctx Calling context
     * @param display Window
     */
    void SRSCALL MainMenuBar(MainMenuBarContext* ctx, SR_WM_display* display);
} // namespace srose::ui::main_menu


#endif
