/**
 * @file move.hpp
 * @author HenryAWE
 * @brief Move component
 */

#ifndef SROSE_PLAYER_COMPONENT_move_hpp_
#define SROSE_PLAYER_COMPONENT_move_hpp_


namespace srose::player::component
{
    struct Move
    {
        float speed = 0;
        float direction = 0;
    };
} // namespace srose::player::component


#endif
