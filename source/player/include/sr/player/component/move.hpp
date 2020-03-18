/**
 * @file move.hpp
 * @author HenryAWE
 * @brief Move component
 */

#ifndef SROSE_PLAYER_COMPONENT_move_hpp_
#define SROSE_PLAYER_COMPONENT_move_hpp_

#include "component.hpp"


namespace srose::player::component
{
    struct Move : public Component<Move>
    {
        Move() noexcept = default;
        Move(const Move& other) noexcept = default;
        Move(float speed_, float direction_) noexcept
            : speed(speed_), direction(direction_) {}

        float speed = 0;
        float direction = 0;
    };
} // namespace srose::player::component


#endif
