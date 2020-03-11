/**
 * @file main_loop.cpp
 * @author HenryAWE
 * @brief Main loop
 */

#include "main_loop.hpp"
#include <chrono>
#include <thread>
#include <sr/wm/event.h>
#include <sr/wm/winmgr.hpp>
#include <sr/ui/gui/uimgr.hpp>
#include <sr/wm/input.hpp>


static bool g_loop = true;

void DoMainLoop()
{
    using namespace srose;
    using namespace srose::ui;
    auto& renderer =  *wm::GetRenderer();
    auto& gui = *ui::GetUIManager();
    auto& input = *wm::GetInputManager();
    
    /*Event processing*/
    input.ProcessAllEvent();
    g_loop = static_cast<bool>(input);
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
    renderer.Render();
    SR_WM_RenderFrame();
    renderer.Present();
}

int SRSCALL BeginMainLoop(int fps)
{ // Prepare for future Emscripten porting
    using namespace std;

    g_loop = true;
    auto frame_sec(1000ms / static_cast<float>(fps));

    while(g_loop)
    {
        auto time_begin = chrono::high_resolution_clock::now();
        DoMainLoop();

        this_thread::sleep_until(time_begin + frame_sec);
    }

    return EXIT_SUCCESS;
}
