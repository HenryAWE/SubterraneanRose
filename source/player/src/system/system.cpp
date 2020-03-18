/**
 * @file system.hpp
 * @author HenryAWE
 * @brief Systems
 */

#include <sr/player/system/system.hpp>


namespace srose::player::system
{
    SystemManager::SystemManager(entity::EntityManager* pmgr)
        : m_pmgr(pmgr) {}

    void SystemManager::UpdateAll()
    {
        for(auto& i : m_systems)
        {
            if(i.second) i.second->Update(*m_pmgr);
        }
    }
} // namespace srose::player::system
