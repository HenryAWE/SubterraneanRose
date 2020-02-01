/**
 * @file main_loop.cpp
 * @author HenryAWE
 * @brief Main loop
 */

#include <glad/glad.h>
#include "main_loop.hpp"
#include <cstdlib>
#include <sr/wm/event.h>
#include <imgui.h>
#include "main_menu/main_menu_bar.hpp"
#include <sr/wm/winmgr.hpp>
#include <sr/locale/locale.hpp>
#include "gui/uimgr.hpp"
#include "i18n/i18n.hpp"
#include "gui/editor/editor_window.hpp"
#include "gui/mainmenu.hpp"


int SRSCALL main_loop(SR_WM_display* display)
{
    using namespace srose;
    using namespace srose::ui;

    auto& renderer =  *wm::GetRenderer();
    auto& gui = *ui::GetUIManager();

    main_menu::MainMenuBar main_menu(display);
    DebugUI dbg_ui(&main_menu.show_debug_ui);
    EditorWindow ed;
    MainMenu mm;

    int loop = SDL_TRUE;
    while(loop)
    {
        /*Event processing*/
        loop = SR_WM_ProcessEvent();
        SR_WM_NewFrame();
        try
        {
            main_menu.Update();
            mm.Update();

            if (main_menu.show_imgui_demo)
                ImGui::ShowDemoWindow(&main_menu.show_imgui_demo);
            dbg_ui.Update();
            ed.Update();
            gui.Update();
        }
        catch(...)
        {
            SR_WM_EndFrame();
            throw;
        }
        SR_WM_EndFrame();

        /*Rendering */
        renderer.ClearScreen();
        SR_WM_RenderFrame();
        renderer.Present();
    }

    return EXIT_SUCCESS;
}
