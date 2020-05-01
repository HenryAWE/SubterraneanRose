/**
 * @file main_loop.hpp
 * @author HenryAWE
 * @brief Main loop
 */

#ifndef SROSE_PLAYER_main_loop_h_internal_
#define SROSE_PLAYER_main_loop_h_internal_

#include <sr/core/macros.hpp>
#include <sr/wm/window.hpp>


int SRSCALL BeginMainLoop(srose::wm::Window& window, int fps = 60);


#endif
