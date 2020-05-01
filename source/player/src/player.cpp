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
    static bool g_demo_initialized = false;

    void ShowDemoWindow(wm::Window& window, bool* p_open)
    {
        auto& uimgr = *ui::GetUIManager();

        if(!g_demo_initialized)
        {
            uimgr.widget_tree.emplace_at(
                "srose.player.demo",
                std::make_shared<PlayerDemoWindow>(window.GetRenderer())
            );
            g_demo_initialized = true;
        }
        auto ptr = uimgr.widget_tree["srose.player.demo"].get();
        static_cast<PlayerDemoWindow*>(ptr)->open = true;
        ptr->Update();
        if(p_open)
            *p_open = static_cast<PlayerDemoWindow*>(ptr)->open;
    }

    void RenderDemoWindow()
    {
        if(!g_demo_initialized)
            return;

        auto& uimgr = *ui::GetUIManager();
        auto ptr = uimgr.widget_tree["srose.player.demo"].get();
        if(static_cast<PlayerDemoWindow*>(ptr)->open)
            static_cast<PlayerDemoWindow*>(ptr)->Render();
    }
#endif

    void ReleaseUIData() noexcept
    {
#ifndef SROSE_DISABLE_DEMO
        if(g_demo_initialized)
        {
            g_demo_initialized = false;
            ui::GetUIManager()->widget_tree.erase_at("srose.player.demo");
        }
#endif
    }
} // namespace srose::player

