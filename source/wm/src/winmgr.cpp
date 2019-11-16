/**
 * @file winmgr.cpp
 * @author HenryAWE
 * @brief Window Manager
 */

#include <glad/glad.h>
#include <sr/wm/winmgr.hpp>
#include <SDL_video.h>
#include <sr/gpu/opengl3/renderer.hpp>


namespace srose::wm
{
    static std::unique_ptr<gpu::Renderer> g_renderer;

    gpu::Renderer* SRSCALL CreateRenderer(SR_WM_display* display)
    {
        g_renderer = std::make_unique<gpu::opengl3::Renderer>(display);

        return g_renderer.get();
    }

    void SRSCALL DestroyRenderer()
    {
        g_renderer.reset();
    }

    gpu::Renderer* SRSCALL GetRenderer()
    {
        return g_renderer.get();
    }
} // namespace srose::wm
