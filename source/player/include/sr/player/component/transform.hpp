/**
 * @file transform.hpp
 * @author HenryAWE
 * @brief Transform component
 */

#ifndef SROSE_PLAYER_COMPONENT_transform_hpp_
#define SROSE_PLAYER_COMPONENT_transform_hpp_

#include <glm/vec2.hpp>
#include "component.hpp"


namespace srose::player::component
{
    struct Transform : public Component<Transform>
    {
        Transform(glm::vec2 position_ = glm::vec2(0), glm::vec2 scale_ = glm::vec2(1), float rotation_ = 0.0f) noexcept
            : position(position_),
            scale(scale_),
            rotation(rotation_) {}

        glm::vec2 position;
        glm::vec2 scale;
        float rotation; // In radians!!!
    };
} // namespace srose::player::component


#endif
