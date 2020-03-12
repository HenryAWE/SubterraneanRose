/**
 * @file player.hpp
 * @author HenryAWE
 * @brief Subterranean Rose Project Player module
 */

#ifndef SROSE_PLAYER_player_hpp_
#define SROSE_PLAYER_player_hpp_


namespace srose::player
{
#ifndef SROSE_DISABLE_DEMO
    void ShowDemoWindow(bool* p_open = nullptr);

    void RenderDemoWindow();
#endif

    void ReleaseUIData() noexcept;
} // namespace srose::player


#endif
