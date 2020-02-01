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
#include <sr/wm/winmgr.hpp>
#include "gui/uimgr.hpp"


int SRSCALL main_loop(SR_WM_display* display)
{
    using namespace srose;
    using namespace srose::ui;

    auto& renderer =  *wm::GetRenderer();
    auto& gui = *ui::GetUIManager();

    int loop = SDL_TRUE;
    while(loop)
    {
        /*Event processing*/
        loop = SR_WM_ProcessEvent();
        SR_WM_NewFrame();
        try
        {
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
