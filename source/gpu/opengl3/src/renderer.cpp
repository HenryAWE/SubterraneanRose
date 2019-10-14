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
} // namespace srose::gpu::opengl3
