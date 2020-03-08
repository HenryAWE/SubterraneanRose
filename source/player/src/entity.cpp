/**
 * @file entity.cpp
 * @author HenryAWE
 * @brief Entitiy management
 */

#include <sr/player/entity.hpp>


namespace srose::player
{
    void SpriteEntity::Render(gpu::Renderer& ren)
    {
        m_sprite.position(m_position);
        m_sprite.scale(m_scale);
        m_sprite.rotation_radians(m_rotation);
        ren.AddSprite(m_sprite);
    }
} // namespace srose::player
