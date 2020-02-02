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

    /**
     * @brief Create renderer
     * 
     * @param display The instance of the display
     * @return gpu::Renderer* CreateRenderer The created renderer
     */
    gpu::Renderer* SRSCALL CreateRenderer(SR_WM_display* display);
    void SRSCALL DestroyRenderer() noexcept;

    [[nodiscard]]
    gpu::Renderer* SRSCALL GetRenderer() noexcept;
} // namespace srose::wm


#endif
