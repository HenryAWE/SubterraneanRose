/**
 * @file main_loop.h
 * @author HenryAWE
 * @brief Main loop
 */

#ifndef SROSE_PLAYER_main_loop_h_internal_
#define SROSE_PLAYER_main_loop_h_internal_


#include <sr/sys/macros.h>
#include <sr/wm/display.h>

#ifdef __cpluscplus
extern "C" {
#endif

int SRSCALL main_loop(SR_WM_display* display);

#ifdef __cpluscplus
}
#endif

#endif
