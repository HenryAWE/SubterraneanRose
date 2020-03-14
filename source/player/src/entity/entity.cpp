/**
 * @file entity.cpp
 * @author HenryAWE
 * @brief Entitiy management
 */

#include <sr/player/entity/entity.hpp>
#include <cassert>


namespace srose::player::entity
{
    const Entity::Id Entity::Id::INVALID = Entity::Id(-1);

    Entity::Entity(EntityManager* pmgr, Id id)
        : m_pmgr(pmgr), m_id(id)
    {
        assert(m_pmgr);
    }

    Entity::~Entity()
    {

    }

    EntityManager::EntityManager(std::size_t reserve)
    {
        m_entities.reserve(reserve);
        m_entities_life.reserve(reserve);

        // An empty entity to keep the vector always valid
        m_entities.emplace_back(this, Entity::Id(Entity::Id::INVALID));
        m_entities_life.emplace_back(0);
    }

    Entity EntityManager::CreateEntity()
    {
        if(m_freed.empty())
        {
            auto index = static_cast<std::int32_t>(m_entities.size());
            auto& e = m_entities.emplace_back(this, Entity::Id(index++, 1));
            m_entities_life.emplace_back(1);
            assert(m_entities.size() == m_entities_life.size());

            return e;
        }
        else
        {
            auto index = static_cast<std::int32_t>(m_freed.top());
            auto& e = m_entities[index] = Entity(
                this,
                Entity::Id(index, m_entities_life[index])
            );
            m_freed.pop();

            return e;
        }
    }
    void EntityManager::DestroyEntity(Entity::Id id) noexcept
    {
        if(!ValidateEntity(id))
            return;
        auto index = id.index();
        for(auto& i : m_components)
            i.erase(id.index());
        m_entities[index].m_id = Entity::Id::INVALID;
        ++m_entities_life[index];
        m_freed.push(index);
    }

    bool EntityManager::ValidateEntity(Entity::Id id) noexcept
    {
        if(id == Entity::Id::INVALID)
            return false;
        auto index= id.index();
        if(index >= m_entities.size())
            return false;
        return m_entities[index].GetId() != Entity::Id::INVALID;
    }

    Entity EntityManager::GetEntity(Entity::Id id) const
    {
        auto index = id.index();
        if(index >= m_entities.size())
            return m_entities[0];
        if(m_entities[index].GetId() == id)
            return m_entities[index];
        else
            return m_entities[0];
    }
} // namespace srose::player::entity
