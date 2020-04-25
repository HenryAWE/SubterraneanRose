/**
 * @file stage.hpp
 * @author HenryAWE
 * @brief Stage
 */

#ifndef SROSE_PLAYER_stage_hpp_
#define SROSE_PLAYER_stage_hpp_

#include <memory>
#include <list>
#include <glm/glm.hpp>
#include "world.hpp"


namespace srose::player
{
    class Stage
    {
        glm::ivec2 m_size = glm::ivec2(0, 0);
    public:
        Stage(glm::ivec2 size);

        ~Stage();

        [[nodiscard]]
        glm::ivec2 GetSize() const noexcept { return m_size; }

        World world;
    };
} // namespace srose::player


#endif
