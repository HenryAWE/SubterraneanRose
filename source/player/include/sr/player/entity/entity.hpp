/**
 * @file entity.hpp
 * @author HenryAWE
 * @brief Entitiy management
 */

#ifndef SROSE_PLAYER_ENTITY_entity_hpp_
#define SROSE_PLAYER_ENTITY_entity_hpp_

#include <memory>
#include <vector>
#include <cstdint>
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

    class EntityManager
    {
        std::vector<Entity> m_entities;
        std::vector<std::int32_t> m_entities_life;
    public:
        EntityManager(std::size_t reserve = 1024);

        Entity CreateEntity();
        void DestroyEntity(Entity::Id id) noexcept;

        bool ValidateEntity(Entity::Id id) noexcept;

        Entity GetEntity(Entity::Id id) const;
    };
} // namespace srose::player::entity


#endif
