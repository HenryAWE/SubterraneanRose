/**
 * @file main_menu_bar.hpp
 * @author HenryAWE
 * @brief Main menu bar
 */

#ifndef SROSE_UI_MAIN_MENU_main_menu_bar_hpp_
#define SROSE_UI_MAIN_MENU_main_menu_bar_hpp_

#include <sr/core/macros.h>
#include <sr/wm/display.h>


namespace srose::ui::main_menu
{
    struct MainMenuBarContext
    {
        bool show_imgui_demo = false;
    };

    /**
     * @brief Main menu
     * 
     * @param ctx Calling context
     * @param display Window
     */
    void SRSCALL MainMenuBar(MainMenuBarContext* ctx, SR_WM_display* display);
} // namespace srose::ui::main_menu


#endif