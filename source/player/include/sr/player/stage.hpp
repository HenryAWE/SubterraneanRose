/**
 * @file stage.hpp
 * @author HenryAWE
 * @brief Stage
 */

#ifndef SROSE_PLAYER_stage_hpp_
#define SROSE_PlAYER_stage_hpp_

#include <memory>
#include <list>
#include <glm/glm.hpp>
#include "world.hpp"


namespace srose::gpu
{
    class Renderer;
    class Scene;
} // namespace srose::gpu

namespace srose::player
{
    class Stage
    {
        gpu::Renderer* m_ren;
        glm::ivec2 m_size = glm::ivec2(0, 0);
        std::unique_ptr<gpu::Scene> m_scene;
    public:
        Stage(glm::ivec2 size, gpu::Renderer* ren);

        ~Stage();

        [[nodiscard]]
        glm::ivec2 GetSize() const noexcept { return m_size; }
        [[nodiscard]]
        gpu::Scene& GetScene() const noexcept { return *m_scene; }

        World world;

        void Render();
    };
} // namespace srose::player


#endif
