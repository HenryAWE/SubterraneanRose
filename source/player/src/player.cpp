/**
 * @file player.cpp
 * @author HenryAWE
 * @brief Subterranean Rose Project Player module
 */

#include <sr/player/player.hpp>
#ifndef SROSE_DISABLE_DEMO
#   include <sr/ui/uimgr.hpp>
#   include "demo.hpp"
#endif


namespace srose::player
{
#ifndef SROSE_DISABLE_DEMO
    static std::shared_ptr<PlayerDemoWindow> g_demo_window;
    static bool g_demo_initialized = false;

    void ShowDemoWindow(wm::Window& window, bool* p_open)
    {
        auto& uimgr = ui::UIManager::GetInstance();

        if(!g_demo_initialized)
        {
            g_demo_window = std::make_shared<PlayerDemoWindow>(window.GetRenderer());
            g_demo_initialized = true;
        }
        g_demo_window->Open();
        g_demo_window->Update();
        if(p_open)
            *p_open = g_demo_window->visible();
    }

    void RenderDemoWindow()
    {
        if(!g_demo_initialized)
            return;

        if(g_demo_window->visible())
            g_demo_window->Render();
    }
#endif

    void ReleaseUIData() noexcept
    {
#ifndef SROSE_DISABLE_DEMO
        if(g_demo_initialized)
        {
            g_demo_window.reset();
            g_demo_initialized = false;
        }
#endif
    }
} // namespace srose::player

