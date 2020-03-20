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
#include <sr/player/component/component.hpp>
#include "../config.hpp"


namespace srose::player::entity
{
    class EntityManager;

    class Entity
    {
    public:
        /* ID of the entity */
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

    private:
        friend class EntityManager;

        EntityManager* m_pmgr = nullptr;
        Id m_id;
    };

    template <typename Com>
    using ComponentHandle = Com*;

    class EntityManager
    {
        // BitMask type, the index of it is the Component::GetFamily()
        typedef std::bitset<SROSE_PLAYER_ECS_MAXCOMPONENTS> BitMask;

        // References to entities, the index of the vector is the GetId().index() of the entity
        std::vector<Entity> m_entities;
        // Indexes of freed entities
        std::stack<std::size_t> m_freed;
        struct EntityData
        {
            std::int32_t life = 0; // Life of the entity, will increase by 1 every time the entity is destroyed
            BitMask mask; // Component mask

            EntityData(std::int32_t life_) noexcept
                : life(life_), mask() {}
        };
        // Entities' data, the index of the vector is the GetId().index() of the entity
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
        // Pool allocators for components, the index of the vector is the Component::GetFamily()
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

        /**
         * @brief Assign a component to an entity
         * 
         * @tparam Com Component type
         * @param id Entity ID
         * @param args Arguments passed to the constructor of the component
         * @return ComponentHandle<Com> Null if the id is invalid
         */
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
        /**
         * @brief Remove a commponent from an entitiy, nothing will happen if the id is invalid
         * 
         * @tparam Com Component type
         * @param id Entity ID
         */
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

        /**
         * @brief Get a component assigned to an entity
         * 
         * @tparam Com Component type
         * @param id Entity ID
         * @return ComponentHandle<Com> Null if the id is invalid or the component not exist
         */
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

        template <typename... Coms>
        static BitMask GetComponentMask()
        {
            if constexpr(sizeof...(Coms)==0)
            {
                return BitMask();
            }
            else
            {
                static_assert((... | std::is_base_of_v<component::BaseComponent, Coms>));
                return (... | BitMask().set(Coms::GetFamily()));
            }
        }

        BitMask GetEntityMask(Entity::Id id)
        {
            if(!ValidateEntity(id))
                return BitMask();
            return m_entities_data[id.index()].mask;
        }

        template <typename... Coms>
        std::tuple<ComponentHandle<Coms>...> GetComponentTuple(Entity::Id id)
        {
            typedef std::tuple<ComponentHandle<Coms>...> ReturnType;
            if(!ValidateEntity(id))
                return ReturnType((sizeof(Coms), nullptr)...);
            auto index = id.index();
            auto mask = GetComponentMask<Coms...>();
            if((m_entities_data[index].mask & mask) != mask)
                return ReturnType((sizeof(Coms), nullptr)...);
            return std::make_tuple(static_cast<Coms*>(m_components[Coms::GetFamily()][id.index()].get())...);
        }

        class ComponentIterator
        {
            EntityManager* m_pmgr;
            std::size_t m_index;
            BitMask m_mask;
        public:
            ComponentIterator(EntityManager* pmgr, BitMask mask_)
                : m_pmgr(pmgr), m_index(0), m_mask(mask_) {}
            ComponentIterator(EntityManager* pmgr, std::size_t index, BitMask mask_)
                : m_pmgr(pmgr), m_index(index), m_mask(mask_) {}
            ComponentIterator(const ComponentIterator& other) = default;

            [[nodiscard]]
            Entity operator*() const noexcept { return m_pmgr->m_entities[m_index]; }
            ComponentIterator& operator++() noexcept { Next(); return *this; }
            [[nodiscard]]
            bool operator!=(const ComponentIterator& rhs) const
            {
                return m_index != rhs.m_index;
            }
            [[nodiscard]]
            operator bool() const noexcept
            {
                return m_index < m_pmgr->m_entities.size();
            }

            [[nodiscard]]
            BitMask mask() const noexcept { return m_mask; }

        private:
            void Next() noexcept
            {
                if(!*this)
                    return;
                ++m_index;
                if(!*this)
                    return;
                if(
                    m_pmgr->m_entities[m_index].m_id == Entity::Id::INVALID ||
                    ((m_pmgr->m_entities_data[m_index].mask & m_mask) != m_mask)
                ) {
                    Next();
                }
            }
        };

        template <typename... Coms>
        struct TypedView
        {
            EntityManager* const pmgr;
            BitMask mask;
            TypedView(EntityManager* pmgr_) noexcept
                : pmgr(pmgr_), mask(GetComponentMask<Coms...>()) {}

            typedef ComponentIterator iterator;
            iterator begin() { return ++iterator(pmgr, 0, mask); }
            iterator end() { return iterator(pmgr, pmgr->m_entities.size(), mask); }
        };

        template <typename... Coms>
        TypedView<Coms...> EntitiesWithComponent()
        {
            return TypedView<Coms...>(this);
        }
    };
} // namespace srose::player::entity


#endif
