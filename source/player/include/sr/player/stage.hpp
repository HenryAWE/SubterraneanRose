/**
 * @file stage.hpp
 * @author HenryAWE
 * @brief Stage
 */

#ifndef SROSE_PLAYER_stage_hpp_
#define SROSE_PlAYER_stage_hpp_

#include <memory>
#include <list>
#include <sr/gpu/renderer.hpp>
#include "entity.hpp"


namespace srose::player
{
    class Stage
    {
        glm::ivec2 m_size = glm::ivec2(0, 0);
        std::list<std::unique_ptr<Entity>> m_entities;
        std::unique_ptr<gpu::Scene> m_scene;
    public:
        Stage(glm::ivec2 size, gpu::Renderer& ren);

        [[nodiscard]]
        glm::ivec2 GetSize() const noexcept { return m_size; }
        [[nodiscard]]
        constexpr const std::list<std::unique_ptr<Entity>>& GetEntities() const noexcept { return m_entities; }
        [[nodiscard]]
        const gpu::Scene& GetScene() const noexcept { return *m_scene; }

        void Update();
        void Render(gpu::Renderer& ren);

        void AddEntity(std::unique_ptr<Entity> entity);

    protected:
        void DoRender(gpu::Renderer& ren);
    };
} // namespace srose::player


#endif
