/**
 * @file main_loop.cpp
 * @author HenryAWE
 * @brief 
 */

#include "main_loop.hpp"
#include <cstdlib>
#include <sr/wm/event.h>
#include <imgui.h>
#include "main_menu/main_menu_bar.hpp"


int SRSCALL main_loop(SR_WM_display* display)
{
    using namespace srose::ui;

    main_menu::MainMenuBarContext main_menu_ctx{};

    int loop = SDL_TRUE;
    while(loop)
    {
        /*Event processing*/
        loop = SR_WM_ProcessEvent();
        SR_WM_NewFrame();

        main_menu::MainMenuBar(&main_menu_ctx, display);

        if(main_menu_ctx.show_imgui_demo)
            ImGui::ShowDemoWindow();
        SR_WM_EndFrame();

        /*Rendering */
        SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
        SDL_RenderClear(display->renderer);
        SR_WM_RenderFrame();
        SDL_RenderPresent(display->renderer);
    }

    return EXIT_SUCCESS;
}
