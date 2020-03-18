/**
 * @file render_system.cpp
 * @author HenryAWE
 * @brief Render system and components
 */

#include "render_system.hpp"
#include <sr/gpu/opengl3/renderer.hpp>


namespace srose::gpu::opengl3
{
    RenderSystem::RenderSystem(Renderer* ren)
        : m_ren(ren) {}

    void RenderSystem::Update(
            player::entity::Entity e,
            player::component::Transform& tr,
            player::component::Image& img
    ) {
        gpu::Sprite sp;
        sp.position(tr.position);
        sp.scale(tr.scale);
        sp.rotation(tr.rotation);
        sp.SetTexture(img.image);

        m_ren->AddSprite(sp);
    }
} // namespace srose::gpu::opengl3
