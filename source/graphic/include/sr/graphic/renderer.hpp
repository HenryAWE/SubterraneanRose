/**
 * @file renderer.hpp
 * @author HenryAWE
 * @brief Renderer
 */

#ifndef SROSE_GRAPHIC_renderer_hpp_
#define SROSE_GRAPHIC_renderer_hpp_

#include <glm/glm.hpp>
#include <stack>
#include <sr/wm/window.hpp>
#include <sr/filesystem/filesystem.hpp>
#include <sr/util/string_tree.hpp>
#include "image.hpp"
#include "sprite.hpp"
#include <sr/player/stage.hpp>
#include "stage_data.hpp"


namespace srose::graphic
{
    /**
     * @brief Renderer base
     */
    class Renderer
    {
        wm::Window* m_window = nullptr;
        std::stack<glm::vec4> m_color_stack;
    protected:
        Renderer(wm::Window& window);

    public:
        virtual ~Renderer() noexcept;

        [[nodiscard]]
        constexpr wm::Window& GetWindow() noexcept { return *m_window; }
        [[nodiscard]]
        constexpr const wm::Window& GetWindow() const noexcept { return *m_window; }

        void PushColor(const glm::vec4& color);
        void PopColor() noexcept;
        const glm::vec4& GetCurrentColor() const noexcept;

        virtual void ClearScreen() = 0;
        virtual void Render() {};
        virtual void Present() {}

        util::string_tree<std::shared_ptr<Texture>, '/'> textures;

        std::unique_ptr<Texture> CreateTexture() { return std::unique_ptr<Texture>(NewTexture()); }
        std::unique_ptr<Texture> CreateScreenTexture(glm::ivec2 size) { return std::unique_ptr<Texture>(NewScreenTexture(size)); }

        bool LoadTextureFromFile(std::string_view as, const filesystem::path& file);

        virtual void AddSprite(const Sprite& sp) = 0;
        virtual void RenderSprite(glm::vec2 viewport, bool clear = true) = 0;
        virtual void ClearSprite() = 0;

        virtual void AddRenderData(player::Stage& stage) = 0;
        virtual void RenderStage(player::Stage& stage) = 0;

        virtual void AddRenderSystem(player::system::SystemManager& smgr) = 0;

        /**
         * @brief Show the renderer's demo window
         * @remark The implementation of the base class does nothing
         * 
         * @param p_open [Out] False if the window was closed
         */
        virtual void ShowDemoWindow(bool* p_open = nullptr) {};

        /**
         * @brief Release the UI data, used by the finalizing code since UI manager will be released earlier
         */
        virtual void ReleaseUIData() noexcept {};

    protected:
        virtual Texture* NewTexture() = 0;
        virtual Texture* NewScreenTexture(glm::ivec2 size) = 0;

        friend class wm::Window;
        virtual void InitImGuiRenderer() = 0;
        virtual void ShutdownImGuiRenderer() = 0;
        virtual void NewImGuiFrame() = 0;
        virtual void RenderImGuiFrame() = 0;
    };
} // namespace srose::graphic


#endif
