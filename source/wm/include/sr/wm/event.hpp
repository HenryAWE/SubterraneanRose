/**
 * @file event.hpp
 * @author HenryAWE
 * @brief Event handler
 */

#ifndef SROSE_WM_event_hpp_
#define SROSE_WM_event_hpp_

#include <sr/core/macros.hpp>
#include <sr/wm/window.hpp>


namespace srose::wm
{
    /**
     * @brief Initialize event handler
     * 
     * @param window The reference of the previously created window
     * @return int SR_WM_InitEventSystem -1 means an error occurred, 0 means succeeded
     */
    int SRSCALL InitEventSystem(wm::Window& window);

    /**
     * @brief Quit the event system and release the data
     */
    void SRSCALL QuitEventSystem();

    /**
     * @brief Begin a new frame
     */
    void SRSCALL NewFrame();

    /**
     * @brief End a frame
     */
    void SRSCALL EndFrame();

    /**
     * @brief Update vertex data and draw the UI
     * @remark SDL_WM_EndFrame() must be called before it!
     */
    void SRSCALL RenderFrame();
} // namespace srose::wm


#endif
