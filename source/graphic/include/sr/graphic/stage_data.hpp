/**
 * @file stage_data.hpp
 * @author HenryAWE
 * @brief Stage render data
 */

#ifndef SROSE_GRAPHIC_stage_data_hpp_
#define SROSE_GRAPHIC_stage_data_hpp_

#include <sr/player/component/component.hpp>
#include "image.hpp"


namespace srose::graphic
{
    struct StageRenderData : public player::component::Component<StageRenderData>
    {
        std::unique_ptr<Texture> screen_texture;

        StageRenderData(
            std::unique_ptr<Texture> screen_texture_
        ) : screen_texture(std::move(screen_texture_)) {}
    };
} // namespace srose::graphic


#endif
