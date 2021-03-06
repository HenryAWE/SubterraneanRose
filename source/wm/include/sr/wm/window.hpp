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
#include <imgui.h>


namespace srose::graphic
{
    class Renderer;
} // namespace srose::graphic

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

        graphic::Renderer& GetRenderer();
        ImGuiContext* GetContext() const noexcept { return m_imctx; }

        void NewImGuiFrame();
        void EndImGuiFrame();
        void RenderImGuiFrame();

    private:
        handle_type m_handle = nullptr;
        SDL_GLContext m_glctx = nullptr;
        ImGuiContext* m_imctx = nullptr;
        graphic::Renderer* m_renderer = nullptr;

        void CreateDisplay(
            glm::ivec2 size,
            const std::string& title,
            int display_index,
            int flags
        );
        void CreateRenderer();
        void CreateImGuiContext();
        void DestroyRenderer() noexcept;
        void DestroyDisplay() noexcept;
        void DestroyImGuiContext() noexcept;
    };
} // namespace srose::wm


#endif
