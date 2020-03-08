/**
 * @file stage.hpp
 * @author HenryAWE
 * @brief Stage
 */

#ifndef SROSE_PLAYER_stage_hpp_
#define SROSE_PlAYER_stage_hpp_

#include <list>
#include <sr/gpu/renderer.hpp>
#include "entity.hpp"


namespace srose::player
{
    class Stage
    {
        std::list<Entity> m_entities;
    public:
        [[nodiscard]]
        constexpr const std::list<Entity>& GetEntities() const noexcept { return m_entities; }

        void Update();
        void Render(gpu::Renderer& ren);
    };
} // namespace srose::player


#endif
