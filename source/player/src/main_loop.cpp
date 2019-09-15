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
        loop = SR_WM_ProcessEvent();
        SR_WM_NewFrame();
        ImGui::ShowDemoWindow();
        SR_WM_EndFrame();
        SR_WM_RenderFrame();
        SDL_RenderPresent(display->renderer);
    }

    return EXIT_SUCCESS;
}
