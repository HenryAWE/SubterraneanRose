/**
 * @file window.hpp
 * @author HenryAWE
 * @brief Window
 */

#ifndef SROSE_WM_window_hpp_
#define SROSE_WM_window_hpp_

#include <SDL.h>
#include <string>
#include <glm/vec2.hpp>

namespace srose::gpu
{
    class Renderer;
} // namespace srose::gpu

namespace srose::wm
{
    class Window
    {
    public:
        typedef SDL_Window* handle_type;

        Window();
        Window(const Window&) = delete;

        ~Window() noexcept;

        void Create(
            glm::ivec2 size,
            const std::string& title,
            int display_index = 0,
            int flags = 0
        );
        void Destroy() noexcept;

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr SDL_GLContext glctx() const noexcept { return m_glctx; }

        void SetTitle(const std::string& title);
        void SetVSync(bool vsync);

        gpu::Renderer& GetRenderer();

    private:
        handle_type m_handle = nullptr;
        SDL_GLContext m_glctx = nullptr;
        gpu::Renderer* m_renderer = nullptr;

        void CreateDisplay(
            glm::ivec2 size,
            const std::string& title,
            int display_index,
            int flags
        );
        void CreateRenderer();
        void DestroyRenderer() noexcept;
        void DestroyDisplay() noexcept;
    };
} // namespace srose::wm


#endif
