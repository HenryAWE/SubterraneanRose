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
        std::map<std::size_t, std::unique_ptr<component::BaseComponent>> m_global_components;
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

        template <typename Com, typename... Args>
        void AddGlobalComponent(Args&&... args)
        {
            m_global_components.emplace(std::make_pair(
                Com::GetFamily(),
                std::make_unique<Com>(std::forward<Args>(args)...)
            ));
        }
        template <typename Com>
        void RemoveGlobalComponent() noexcept
        {
            m_global_components.erase(Com::GetFamily());
        }
        template <typename Com>
        entity::ComponentHandle<Com> GetGlobalComponent()
        {
            auto iter = m_global_components.find(Com::GetFamily());
            if(iter == m_global_components.end())
                return nullptr;
            return static_cast<Com*>(iter->second.get());
        }
        template <typename Com>
        bool HasGlobalComponent() noexcept
        {
            return m_global_components.find(Com::GetFamily()) != m_global_components.end();
        }

        void Update();
        void LateUpdate();
        void Render();
    };
} // namespace srose::player

#endif
