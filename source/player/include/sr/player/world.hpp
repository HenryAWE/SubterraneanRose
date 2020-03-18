/**
 * @file world.hpp
 * @author HenryAWE
 * @brief ECS world
 */

#ifndef SROSE_PLAYER_world_hpp_
#define SROSE_PLAYER_world_hpp_

#include "entity/entity.hpp"
#include "component/component.hpp"
#include "system/system.hpp"


namespace srose::player
{
    class World
    {
        entity::EntityManager m_emgr;
        system::SystemManager m_smgr[3];
    public:
        World();

        enum SystemType
        {
            UPDATE = 0,
            LATE_UPDATE,
            RENDER
        };

        [[nodiscard]]
        constexpr entity::EntityManager& GetEntityManager() noexcept { return m_emgr; }
        [[nodiscard]]
        constexpr system::SystemManager& GetSystemManager(SystemType type = UPDATE)noexcept
        {
            return m_smgr[static_cast<std::size_t>(type)];
        }

        void Update();
        void LateUpdate();
        void Render();
    };
} // namespace srose::player

#endif
