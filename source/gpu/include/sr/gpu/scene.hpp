/**
 * @file scene.cpp
 * @author HenryAWE
 * @brief Scene
 */

#ifndef SROSE_GPU_scene_hpp_
#define SROSE_GPU_scene_hpp_

#include <functional>
#include <glm/glm.hpp>
#include "image.hpp"


namespace srose::gpu
{
    class Renderer; // Forward declaration for avoiding recursive inclusion

    class Scene
    {
        glm::ivec2 m_size = glm::ivec2(0);
    public:
        Scene(glm::ivec2 size);

        virtual ~Scene();

        virtual void Render(Renderer& ren, bool to_texture = true) = 0;

        [[nodiscard]]
        virtual const Texture& GetScreenTexture() const = 0;

        [[nodiscard]]
        constexpr glm::ivec2 GetSize() const noexcept { return m_size; }

        using callback_type = void(Renderer&, Scene&);
        void SetRenderCallback(std::function<callback_type> func);

    protected:
        std::function<callback_type> m_render_callback;
    };
} // namespace srose::gpu


#endif
