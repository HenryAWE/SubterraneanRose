/**
 * @file demo.hpp
 * @author HenryAWE
 * @brief Player demo
 */

#ifndef SROSE_PLAYER_demo_hpp_
#define SROSE_PLAYER_demo_hpp_

#include <sr/graphic/image.hpp>
#include <sr/graphic/renderer.hpp>
#include <sr/ui/node.hpp>
#include <sr/player/player.hpp>
#include <sr/player/stage.hpp>


#ifndef SROSE_DISABLE_DEMO

namespace srose::player
{
    class PlayerDemoWindow : public ui::StandaloneNode
    {
        Stage m_stage;
        graphic::Renderer* m_ren;
    public:
        typedef StandaloneNode Base;

        PlayerDemoWindow(graphic::Renderer& ren);

        void Update() override;

        void Render();
    };
} // namespace srose::player

#endif


#endif
