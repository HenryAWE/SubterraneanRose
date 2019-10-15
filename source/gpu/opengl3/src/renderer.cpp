/**
 * @file renderer.cpp
 * @author HenryAWE
 * @brief OpenGL3 renderer
 */

#include <sr/gpu/opengl3/renderer.hpp>
#include <glad/glad.h>
#include <SDL.h>
#include <cassert>

#define ASSERT_CTX() assert(SDL_GL_GetCurrentContext())


namespace srose::gpu::opengl3
{
    Renderer::Renderer(SR_WM_display* disp)
        : m_disp(disp)
    {
        ASSERT_CTX();
        SDL_LogInfo(
            SDL_LOG_CATEGORY_APPLICATION,
            "[OpenGL3] Renderer Information:\n%s",
            Information().c_str()
        );
    }

    void Renderer::ClearScreen(glm::vec4 color) noexcept
    {
        ASSERT_CTX();
        glClearColor(color[0], color[1], color[2], color[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::Present() noexcept
    {
        SDL_GL_SwapWindow(m_disp->win);
    }

    std::string Renderer::Information()
    {
        ASSERT_CTX();

        using namespace std::literals;
        std::string info;
        info += "Vendor: "s + (const char*)glGetString(GL_VENDOR) + '\n';
        info += "Renderer: "s + (const char*)glGetString(GL_RENDERER) + '\n';
        info += "Version: "s + (const char*)glGetString(GL_VERSION) + '\n';

        return std::move(info);
    }
} // namespace srose::gpu::opengl3
