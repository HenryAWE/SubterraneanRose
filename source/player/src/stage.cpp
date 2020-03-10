/**
 * @file stage.cpp
 * @author HenryAWE
 * @brief Stage
 */

#include <sr/player/stage.hpp>


namespace srose::player
{
    Stage::Stage(glm::ivec2 size, gpu::Renderer& ren)
        : m_size(size),
        m_scene(ren.CreateScene(size))
    {
        using namespace std;
        m_scene->SetRenderCallback([this](gpu::Renderer& ren, gpu::Scene&){ DoRender(ren); });
    }

    void Stage::Update()
    {
        for(auto& i : m_entities)
        {
            if(i) i->Update();
        }
    }

    void Stage::Render(gpu::Renderer& ren)
    {
        m_scene->Render(ren, true);
    }

    void Stage::AddEntity(std::unique_ptr<Entity> entity)
    {
        m_entities.emplace_back(std::move(entity));
    }

    void Stage::DoRender(gpu::Renderer& ren)
    {
        for(auto& i : m_entities)
        {
            if(i) i->Render(ren);
        }

        ren.RenderSprite(m_size);
    }
} // namespace srose::player
