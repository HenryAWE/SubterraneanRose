/**
 * @file world.cpp
 * @author HenryAWE
 * @brief ECS world
 */

#include <sr/player/world.hpp>


namespace srose::player
{
    World::World()
        : m_emgr(1024), m_smgr{&m_emgr, &m_emgr, &m_emgr} {}

    void World::Update()
    {
        m_smgr[UPDATE].UpdateAll();
    }
    void World::LateUpdate()
    {
        m_smgr[LATE_UPDATE].UpdateAll();
    }
    void World::Render()
    {
        m_smgr[RENDER].UpdateAll();
    }
} // namespace srose::player
