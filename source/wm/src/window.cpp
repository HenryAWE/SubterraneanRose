/**
 * @file window.cpp
 * @author HenryAWE
 * @brief Window
 */

#include <sr/wm/window.hpp>
#include <stdexcept>
#include <sr/core/init.hpp>
#include <sr/gpu/opengl3/renderer.hpp>


namespace srose::wm
{
    Window::Window() = default;

    Window::~Window() noexcept
    {
        Destroy();
    }

    void Window::Create(
        glm::ivec2 size,
        const std::string& title,
        int display_index,
        int flags
    ) {
        if(m_handle)
            return;
        CreateDisplay(size, title, display_index, flags);
        CreateRenderer();
    }
    void Window::Destroy() noexcept
    {
        if(!m_handle)
            return;
        DestroyRenderer();
        DestroyDisplay();
    }

    gpu::Renderer& Window::GetRenderer()
    {
        return *m_renderer;
    }

    void Window::CreateDisplay(
        glm::ivec2 size,
        const std::string& title,
        int display_index,
        int flags
    ) {
        using namespace std;

        flags |= SDL_WINDOW_ALLOW_HIGHDPI;

        flags |= SDL_WINDOW_OPENGL;
        /* Set the OpenGL attributes for window&context creation*/
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

        SDL_Window* tmp_handle = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED_DISPLAY(display_index),
            SDL_WINDOWPOS_CENTERED_DISPLAY(display_index),
            size.x, size.y,
            flags
        );
        if(!tmp_handle)
        {
            throw runtime_error("SDL_CreateWindow() failed: "s + SDL_GetError());
        }

        SDL_GLContext tmp_glctx = SDL_GL_CreateContext(tmp_handle);
        if(!tmp_glctx)
        {
            SDL_DestroyWindow(tmp_handle);

            throw runtime_error("SDL_GL_CreateContext() failed: "s + SDL_GetError());
        }

        if(core::InitGL() != 0)
        {
            SDL_GL_MakeCurrent(tmp_handle, nullptr);
            SDL_GL_DeleteContext(tmp_glctx);
            SDL_DestroyWindow(tmp_handle);

            throw runtime_error("Failed to initialize OpenGL environment");
        }

        swap(m_handle, tmp_handle);
        swap(m_glctx, tmp_glctx);
        if(tmp_glctx)
            SDL_GL_DeleteContext(tmp_glctx);
        if(tmp_handle)
            SDL_DestroyWindow(tmp_handle);
    }
    void Window::CreateRenderer()
    {
        m_renderer = new gpu::opengl3::Renderer(*this);
    }
    void Window::DestroyRenderer() noexcept
    {
        delete m_renderer;
    }
    void Window::DestroyDisplay() noexcept
    {
        SDL_GL_MakeCurrent(m_handle, nullptr);
        SDL_GL_DeleteContext(m_glctx);
        m_glctx = nullptr;
        SDL_DestroyWindow(m_handle);
        m_handle = nullptr;
    }
} // namespace srose::wm
