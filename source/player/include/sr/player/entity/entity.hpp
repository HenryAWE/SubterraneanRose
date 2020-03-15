/**
 * @file entity.hpp
 * @author HenryAWE
 * @brief Entitiy management
 */

#ifndef SROSE_PLAYER_ENTITY_entity_hpp_
#define SROSE_PLAYER_ENTITY_entity_hpp_

#include <memory>
#include <vector>
#include <stack>
#include <bitset>
#include <map>
#include <type_traits>
#include <cstdint>
#include <boost/pool/pool_alloc.hpp>
#include <sr/player/component/transform.hpp>
#include <sr/player/component/move.hpp>


namespace srose::player::entity
{
    class EntityManager;

    class Entity
    {
    public:
        class Id
        {
            std::uint64_t m_id;
        public:
            constexpr Id() noexcept : m_id(0) {}
            constexpr Id(const Id& other) noexcept = default;
            constexpr Id(std::uint64_t id_) noexcept : m_id(id_) {}
            constexpr Id(std::int32_t index_, std::int32_t version_) noexcept
                : m_id(std::uint64_t(index_) | std::uint64_t(version_)<<32ul) {}

            [[nodiscard]]
            constexpr bool operator==(const Id& rhs) const noexcept { return m_id == rhs.m_id; }
            [[nodiscard]]
            constexpr bool operator!=(const Id& rhs) const noexcept { return m_id != rhs.m_id; }
            [[nodiscard]]
            constexpr bool operator<(const Id& rhs) const noexcept { return m_id < rhs.m_id; }
            [[nodiscard]]
            constexpr bool operator>(const Id& rhs) const noexcept { return m_id > rhs.m_id; }

            [[nodiscard]]
            constexpr std::int64_t id() const noexcept { return m_id; }
            [[nodiscard]]
            constexpr operator std::int64_t() const noexcept { return m_id; }

            [[nodiscard]]
            constexpr std::int32_t index() const noexcept { return m_id & 0xfffffffful; }
            [[nodiscard]]
            constexpr std::int32_t version() const noexcept { return m_id >> 32; }

            static const Id INVALID;
        };

        Entity(EntityManager* pmgr, Id id);
        Entity(const Entity& other) noexcept
            : m_pmgr(other.m_pmgr), m_id(other.m_id) {}

        ~Entity();

        [[nodiscard]]
        constexpr EntityManager* GetManager() const noexcept { return m_pmgr; }
        [[nodiscard]]
        constexpr const Id& GetId() const noexcept { return m_id; }

        [[nodiscard]]
        constexpr bool valid() const noexcept { return m_id == Id::INVALID; }

    private:
        friend class EntityManager;

        EntityManager* m_pmgr = nullptr;
        Id m_id;
    };

    template <typename Com>
    using ComponentHandle = Com*;

    class EntityManager
    {
        typedef std::bitset<SROSE_PLAYER_ECS_MAXCOMPONENTS> BitMask;

        std::vector<Entity> m_entities;
        std::stack<std::size_t> m_freed;
        struct EntityData
        {
            std::int32_t life = 0;
            BitMask mask;

            EntityData(std::int32_t life_) noexcept
                : life(life_), mask() {}
        };
        std::vector<EntityData> m_entities_data;

        Entity& AccommodateEntity();

        class BasePool
        {
        public:
            virtual ~BasePool() = default;
        };
        template <typename Alloc>
        class Pool : public BasePool
        {
        public:
            typedef Alloc allocator_type;

            Alloc allocator;
        };
        std::vector<std::unique_ptr<BasePool>> m_component_pools;

        template <typename T>
        using ComAlloc = boost::pool_allocator<T>;

        template <typename Com>
        ComAlloc<Com>& AccommodateComponentAlloc()
        {
            std::size_t family = Com::GetFamily();
            if(family >= m_component_pools.size())
                m_component_pools.resize(family + 1);
            if(!m_component_pools[family])
                m_component_pools[family] = std::make_unique<Pool<ComAlloc<Com>>>();
            return static_cast<Pool<ComAlloc<Com>>*>(m_component_pools[family].get())->allocator;
        }

        template <typename Key, typename T>
        using MapType = std::map<Key, T>;

        // Stores components
        // The index of this vector is the family of the component
        // The index of the map is the index of the entity
        std::vector<MapType<std::int32_t, std::shared_ptr<component::BaseComponent>>> m_components;
    public:
        EntityManager(std::size_t reserve = 1024);

        Entity CreateEntity();
        void DestroyEntity(Entity::Id id) noexcept;

        bool ValidateEntity(Entity::Id id) noexcept;

        Entity GetEntity(Entity::Id id) const;

        template <typename Com, typename... Args>
        ComponentHandle<Com> AssignComponent(Entity::Id id, Args&&... args)
        {
            if(!ValidateEntity(id))
                return nullptr;

            static_assert(std::is_base_of_v<component::BaseComponent, Com>);
            std::size_t family = Com::GetFamily();
            std::int32_t index = id.index();
            if(family >= m_components.size())
                m_components.resize(family + 1);

            auto ret =  static_cast<Com*>(m_components[family].insert_or_assign(
                index,
                std::allocate_shared<Com>(AccommodateComponentAlloc<Com>(), std::forward<Args>(args)...)
            ).first->second.get());
            m_entities_data[index].mask.set(family);
            
            return ret;
        }
        template <typename Com>
        void RemoveComponent(Entity::Id id)
        {
            if(!ValidateEntity(id))
                return;

            std::size_t family = Com::GetFamily();
            if(family >= m_components.size())
                return;
            m_components[family].erase(id.index());
            m_entities_data[id.index()].mask.set(family, false);
        }

        template <typename Com>
        bool HasComponent(Entity::Id id) noexcept
        {
            if(!ValidateEntity(id))
                return false;
            return m_entities_data[id.index()].mask.test(Com::GetFamily());
        }
        template <typename Com>
        ComponentHandle<Com> GetComponent(Entity::Id id)
        {
            if(!ValidateEntity(id))
                return nullptr;

            std::size_t family = Com::GetFamily();
            if(family >= m_components.size())
                return nullptr;
            auto& com_map = m_components[family];
            auto iter = com_map.find(id.index());
            if(iter == com_map.end())
                return nullptr;
            return static_cast<Com*>(iter->second.get());
        }
    };
} // namespace srose::player::entity


#endif
