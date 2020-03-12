/**
 * @file entity.hpp
 * @author HenryAWE
 * @brief Entitiy management
 */

#ifndef SROSE_PLAYER_ENTITY_entity_hpp_
#define SROSE_PLAYER_ENTITY_entity_hpp_

#include <sr/player/component/transform.hpp>
#include <sr/player/component/move.hpp>


namespace srose::player::entity
{
    class Entity
    {
    public:
        Entity();

        ~Entity();
    };
} // namespace srose::player::entity


#endif
