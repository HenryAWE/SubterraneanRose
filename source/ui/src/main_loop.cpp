/**
 * @file main_loop.cpp
 * @author HenryAWE
 * @brief 
 */

#include "main_loop.hpp"
#include <stdlib.h>
#include <sr/wm/event.h>
#include <imgui.h>


int SRSCALL main_loop(SR_WM_display* display)
{
    int loop = SDL_TRUE;
    while(loop)
    {
        /*Event processing*/
        loop = SR_WM_ProcessEvent();
        SR_WM_NewFrame();
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
