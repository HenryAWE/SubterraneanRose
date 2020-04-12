/**
 * @file renderer.hpp
 * @author HenryAWE
 * @brief Renderer
 */

#ifndef SROSE_GPU_renderer_hpp_
#define SROSE_GPU_renderer_hpp_

#include <glm/glm.hpp>
#include <stack>
#include <sr/wm/display.hpp>
#include <sr/filesystem/filesystem.hpp>
#include <sr/util/string_tree.hpp>
#include "image.hpp"
#include "sprite.hpp"
#include "scene.hpp"
#include <sr/player/system/system.hpp>


namespace srose::gpu
{
    using wm::Display;

    /**
     * @brief Renderer base
     */
    class Renderer
    {
        Display* m_disp = nullptr;
        std::stack<glm::vec4> m_color_stack;
    protected:
        Renderer(Display* disp);

    public:
        virtual ~Renderer() noexcept;

        [[nodiscard]]
        constexpr Display* GetDisplay() const noexcept { return m_disp; }

        void PushColor(const glm::vec4& color);
        void PopColor() noexcept;
        const glm::vec4& GetCurrentColor() const noexcept;

        virtual void ClearScreen() = 0;
        virtual void Render() {};
        virtual void Present() {}

        util::string_tree<std::shared_ptr<Texture>, '/'> textures;

        std::unique_ptr<Texture> CreateTexture() { return std::unique_ptr<Texture>(NewTexture()); }
        std::unique_ptr<Scene> CreateScene(glm::ivec2 size) { return std::unique_ptr<Scene>(NewScene(size)); }

        bool LoadTextureFromFile(std::string_view as, const filesystem::path& file);

        virtual void AddSprite(const Sprite& sp) = 0;
        virtual void RenderSprite(glm::vec2 viewport, bool clear = true) = 0;
        virtual void ClearSprite() = 0;

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
        virtual Scene* NewScene(glm::ivec2 size) = 0;
    };
} // namespace srose::gpu


#endif
