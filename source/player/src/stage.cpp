/**
 * @file stage.cpp
 * @author HenryAWE
 * @brief Stage
 */

#include <sr/player/stage.hpp>


namespace srose::player
{
    Stage::Stage(glm::ivec2 size)
        : m_size(size) {}

    Stage::~Stage() = default;
} // namespace srose::player
