/**
 * @file main_loop.cpp
 * @author HenryAWE
 * @brief Main loop
 */

#include "main_loop.hpp"
#include <chrono>
#include <thread>
#include <sr/wm/event.hpp>
#include <sr/ui/uimgr.hpp>
#include <sr/wm/input.hpp>
#include <sr/graphic/renderer.hpp>


static bool g_loop = true;

void DoMainLoop(srose::wm::Window& window)
{
    using namespace srose;
    using namespace srose::ui;
    auto& renderer =  window.GetRenderer();
    auto& gui = UIManager::GetInstance();
    auto& input = *wm::GetInputManager();
    
    /*Event processing*/
    input.ProcessAllEvent();
    g_loop = static_cast<bool>(input);
    window.NewImGuiFrame();
    try
    {
        gui.Update();
    }
    catch(...)
    {
        window.EndImGuiFrame();
        throw;
    }
    window.EndImGuiFrame();

    /*Rendering */
    renderer.ClearScreen();
    renderer.Render();
    window.RenderImGuiFrame();
    renderer.Present();
}

int SRSCALL BeginMainLoop(srose::wm::Window& window, int fps)
{ // Prepare for future Emscripten porting
    using namespace std;

    g_loop = true;
    auto frame_sec(1000ms / static_cast<float>(fps));

    while(g_loop)
    {
        auto time_begin = chrono::high_resolution_clock::now();
        DoMainLoop(window);

        this_thread::sleep_until(time_begin + frame_sec);
    }

    return EXIT_SUCCESS;
}
