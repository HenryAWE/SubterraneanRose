/**
 * @file move.hpp
 * @author HenryAWE
 * @brief Move system
 */

#ifndef SROSE_PLAYER_SYSTEM_move_hpp_
#define SROSE_PLAYER_SYSTEM_move_hpp_

#include <sr/player/entity/entity.hpp>
#include <sr/player/component/transform.hpp>
#include <sr/player/component/move.hpp>
#include <cmath>


namespace srose::player::system
{
    struct Move
    {
        void Update(entity::Entity, component::Move& m, component::Transform& tr)
        {
            tr.position.x += m.speed * std::cos(m.direction);
            tr.position.y += m.speed * std::sin(m.direction);
        };
    };
} // namespace srose::player::system


#endif
