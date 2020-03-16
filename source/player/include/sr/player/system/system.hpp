/**
 * @file system.hpp
 * @author HenryAWE
 * @brief Systems
 */

#ifndef SROSE_PLAYER_SYSTEM_system_hpp_
#define SROSE_PLAYER_SYSTEM_system_hpp_

#include <tuple>
#include <memory>
#include <typeindex>
#include <vector>
#include <map>
#include <cassert>
#include <sr/player/entity/entity.hpp>


namespace srose::player::system
{
    class SystemManager
    {
        friend class entity::EntityManager;

        class BaseSystemUpdater
        {
        public:
            BaseSystemUpdater() = default;

            virtual ~BaseSystemUpdater() = default;

            virtual void Update(entity::EntityManager& e) = 0;
        };
        template <typename Sys, typename RequiredComponent>
        class SystemUpdater : public BaseSystemUpdater
        {
            Sys m_sys;
        public:
            template <typename... Args>
            SystemUpdater(Args&&... args)
                : m_sys(std::forward<Args>(args)...) {}


            template <std::size_t... Indexes>
            void UpdateImpl(entity::EntityManager& e, std::index_sequence<Indexes...> idx)
            {
                for(const auto& i : e.EntitiesWithComponent<std::tuple_element_t<Indexes, RequiredComponent>...>())
                {
                    auto args = e.GetComponentTuple<std::tuple_element_t<Indexes, RequiredComponent>...>(i.GetId());
                    assert((...&&std::get<Indexes>(args)));
                    m_sys.Update(i, *std::get<Indexes>(args)...);
                }
            }
            void Update(entity::EntityManager& e)
            {
                UpdateImpl(e, std::make_index_sequence<std::tuple_size_v<RequiredComponent>>());
            }
        };
        std::map<std::type_index, std::unique_ptr<BaseSystemUpdater>> m_systems;

        entity::EntityManager* m_pmgr = nullptr;
    public:
        SystemManager(entity::EntityManager* pmgr);

        /**
         * @brief Add a new system
         * 
         * @tparam Sys System type
         * @tparam RequiredComponent A tuple of required components, e.g. std::tuple<MoveComponent, TransformComponent>
         * @param args Arguments passed to the constructor of the system
         */
        template <
            typename Sys,
            typename RequiredComponent = std::tuple<>,
            typename... Args
        >
        void Add(Args&&... args)
        {
            m_systems[typeid(Sys)] = std::make_unique<SystemUpdater<Sys, RequiredComponent>>(std::forward<Args>(args)...);
        }
        template <typename Sys>
        void Remove() noexcept
        {
            m_systems.erase(typeid(Sys));
        }
        template <typename Sys>
        bool Has() noexcept
        {
            return m_systems.count(typeid(Sys));
        }

        void UpdateAll();
    };
} // namespace srose::player::system


#endif
