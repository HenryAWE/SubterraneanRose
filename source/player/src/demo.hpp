/**
 * @file demo.hpp
 * @author HenryAWE
 * @brief Player demo
 */

#ifndef SROSE_PLAYER_demo_hpp_
#define SROSE_PLAYER_demo_hpp_

#include <sr/gpu/image.hpp>
#include <sr/gpu/renderer.hpp>
#include <sr/ui/widget.hpp>
#include <sr/ui/node.hpp>
#include <sr/player/player.hpp>
#include <sr/player/stage.hpp>


#ifndef SROSE_DISABLE_DEMO

namespace srose::player
{
    class PlayerDemoWindow : public ui::StandaloneNode
    {
        Stage m_stage;
        gpu::Renderer* m_ren;
    public:
        typedef StandaloneNode Base;

        PlayerDemoWindow(gpu::Renderer& ren);

        void Update() override;

        void Render();
    };
} // namespace srose::player

#endif


#endif
