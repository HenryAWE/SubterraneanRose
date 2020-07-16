/**
 * @file image.hpp
 * @author HenryAWE
 * @brief Image component
 */

#ifndef SROSE_PLAYER_COMPONENT_image_hpp_
#define SROSE_PLAYER_COMPONENT_image_hpp_

#include "component.hpp"
#include <sr/graphic/image.hpp>


namespace srose::player::component
{
    struct Image : public Component<Image>
    {
        Image(gpu::Texture* image_ = nullptr) noexcept
            : image(image_) {}

        gpu::Texture* image;
    };
} // namespace srose::player::component


#endif
