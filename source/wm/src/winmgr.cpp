/**
 * @file winmgr.cpp
 * @author HenryAWE
 * @brief Window Manager
 */

#include <glad/glad.h>
#include <sr/wm/winmgr.hpp>
#include <SDL_video.h>
#include <sr/gpu/opengl3/renderer.hpp>
#include <string>


namespace srose::wm
{
    void SetWindowSubTitle(SR_WM_display* disp, const char* subtitle)
    {
        std::string title = !subtitle || subtitle[0] == '\0'?
            "Subterranean Rose":
            subtitle + std::string(" | Subterranean Rose");
        SDL_SetWindowTitle(disp->win, title.c_str());
    }

    static std::unique_ptr<gpu::Renderer> g_renderer;

    gpu::Renderer* SRSCALL CreateRenderer(SR_WM_display* display)
    {
        g_renderer = std::make_unique<gpu::opengl3::Renderer>(display);

        return g_renderer.get();
    }

    void SRSCALL DestroyRenderer() noexcept
    {
        g_renderer.reset();
    }

    gpu::Renderer* SRSCALL GetRenderer() noexcept
    {
        return g_renderer.get();
    }
} // namespace srose::wm
