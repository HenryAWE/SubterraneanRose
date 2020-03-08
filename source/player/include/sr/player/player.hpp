/**
 * @file player.hpp
 * @author HenryAWE
 * @brief Subterranean Rose Project Player module
 */

#ifndef SROSE_PLAYER_player_hpp_
#define SROSE_PLAYER_player_hpp_

#include "entity.hpp"


namespace srose::player
{
#ifndef SROSE_DISABLE_DEMO
    void ShowDemoWindow(bool* p_open = nullptr);
#endif

    void ReleaseUIData() noexcept;
} // namespace srose::player


#endif
