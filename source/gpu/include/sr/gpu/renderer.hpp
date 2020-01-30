/**
 * @file renderer.hpp
 * @author HenryAWE
 * @brief Renderer
 */

#ifndef SROSE_GPU_renderer_hpp_
#define SROSE_GPU_renderer_hpp_

#include <glm/glm.hpp>
#include <stack>
#include <sr/filesystem/filesystem.hpp>
#include <sr/util/string_tree.hpp>
#include "image.hpp"


namespace srose::gpu
{
    /**
     * @brief Renderer base
     */
    class Renderer
    {
        std::stack<glm::vec4> m_color_stack;
    protected:
        Renderer();

    public:
        virtual ~Renderer() noexcept;

        void PushColor(const glm::vec4& color);
        void PopColor() noexcept;
        const glm::vec4& GetCurrentColor() const noexcept;

        virtual void ClearScreen() = 0;
        virtual void Present() {}

        util::string_tree<std::shared_ptr<Texture>, '/'> textures;

        virtual std::unique_ptr<gpu::Texture> CreateTexture() = 0;

        bool LoadTextureFromFile(std::string_view as, const filesystem::path& file);
    };
} // namespace srose::gpu


#endif
