/**
 * @file winmgr.hpp
 * @author HenryAWE
 * @brief Windows Manager
 */

#ifndef SROSE_WM_winmgr_hpp_
#define SROSE_WM_winmgr_hpp_

#include <memory>
#include <sr/gpu/renderer.hpp>
#include "display.h"


namespace srose::wm
{
    using ::SR_WM_display;

   gpu::Renderer* SRSCALL CreateRenderer(SR_WM_display* display);
   void SRSCALL DestroyRenderer();

   [[nodiscard]]
   gpu::Renderer* SRSCALL GetRenderer();
} // namespace srose::wm


#endif
