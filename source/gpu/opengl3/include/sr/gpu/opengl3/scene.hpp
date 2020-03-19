/**
 * @file scene.hpp
 * @author HenryAWE
 * @brief OpenGL3 scene renderer
 */

#ifndef SROSE_GPU_OPENGL3_scene_hpp_
#define SROSE_GPU_OPENGL3_scene_hpp_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <sr/gpu/scene.hpp>
#include "buffer.hpp"
#include "texture.hpp"


namespace srose::gpu::opengl3
{
    class Renderer;

    class Scene final : public gpu::Scene
    {
        Framebuffer m_fbo;
        Renderbuffer m_rbo;
        Texture m_screen;
    public:
        Scene(glm::ivec2 size, Renderer* ren);

        void Render(bool to_texture = true) override;

        [[nodiscard]]
        const Texture& GetScreenTexture() const override;

    private:
        [[nodiscard]]
        Renderer* GetRenderer() const noexcept;

        void Generate();
    };
} // namespace srose::gpu::opengl3


#endif
