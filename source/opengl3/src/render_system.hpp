/**
 * @file render_system.hpp
 * @author HenryAWE
 * @brief Render system and components
 */

#ifndef SROSE_GPU_OPENGL3_render_system_hpp_
#define SROSE_GPU_OPENGL3_render_system_hpp_

#include <sr/player/system/system.hpp>
#include <sr/player/component/image.hpp>
#include <sr/player/component/transform.hpp>


namespace srose::graphic::opengl3
{
    class Renderer;

    class RenderSystem
    {
        Renderer* m_ren;
    public:
        RenderSystem(Renderer* ren);

        void Update(
            player::entity::Entity e,
            player::component::Transform& tr,
            player::component::Image& img
        );
    };
} // namespace srose::graphic::opengl3


#endif
