/**
 * @file render_system.cpp
 * @author HenryAWE
 * @brief Render system and components
 */

#include "render_system.hpp"
#include <sr/graphic/opengl3/renderer.hpp>


namespace srose::graphic::opengl3
{
    RenderSystem::RenderSystem(Renderer* ren)
        : m_ren(ren) {}

    void RenderSystem::Update(
            player::entity::Entity e,
            player::component::Transform& tr,
            player::component::Image& img
    ) {
        graphic::Sprite sp;
        sp.position(tr.position);
        sp.scale(tr.scale);
        sp.rotation(tr.rotation);
        sp.SetTexture(img.image);

        m_ren->AddSprite(sp);
    }
} // namespace srose::graphic::opengl3
