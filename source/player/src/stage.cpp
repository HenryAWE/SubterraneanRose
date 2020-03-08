/**
 * @file stage.cpp
 * @author HenryAWE
 * @brief Stage
 */

#include <sr/player/stage.hpp>


namespace srose::player
{
    void Stage::Update()
    {
        for(auto& i : m_entities)
            i.Update();
    }

    void Stage::Render(gpu::Renderer& ren)
    {
        for(auto& i : m_entities)
            i.Render(ren);
    }
} // namespace srose::player
