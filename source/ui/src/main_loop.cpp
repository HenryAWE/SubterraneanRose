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
#include "i18n/i18n.hpp"
#include "gui/editor/editor_window.hpp"


int SRSCALL main_loop(SR_WM_display* display)
{
    using namespace srose;
    using namespace srose::ui;

    auto& renderer =  *wm::GetRenderer();

    main_menu::MainMenuBar main_menu(display);
    DebugUI dbg_ui(&main_menu.show_debug_ui);
    EditorWindow ed;

    int loop = SDL_TRUE;
    while(loop)
    {
        /*Event processing*/
        loop = SR_WM_ProcessEvent();
        SR_WM_NewFrame();

        main_menu.Update();

        if(main_menu.show_imgui_demo)
            ImGui::ShowDemoWindow(&main_menu.show_imgui_demo);
        dbg_ui.Update();
        ed.Update();
        SR_WM_EndFrame();

        /*Rendering */
        renderer.ClearScreen();
        SR_WM_RenderFrame();
        renderer.Present();
    }

    return EXIT_SUCCESS;
}
