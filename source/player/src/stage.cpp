/**
 * @file stage.cpp
 * @author HenryAWE
 * @brief Stage
 */

#include <sr/player/stage.hpp>
#include <cassert>
#include <sr/gpu/renderer.hpp>


namespace srose::player
{
    Stage::Stage(glm::ivec2 size, gpu::Renderer* ren)
        : m_ren(ren), m_size(size), m_scene(ren->CreateScene(size))
    {
        assert(m_ren);
        m_ren->AddRenderSystem(world.GetSystemManager(world.RENDER));

        m_scene->SetRenderCallback([this]() {
            world.Render();
            gpu::Sprite sp;
            sp.scale(glm::vec2(5)) ;
            sp.SetTexture(nullptr);

            m_ren->RenderSprite(m_size, true);
        });
    }

    Stage::~Stage() = default;

    void Stage::Render()
    {
        m_scene->Render();
    }
} // namespace srose::player
