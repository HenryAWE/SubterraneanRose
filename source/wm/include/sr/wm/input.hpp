/**
 * @file input.hpp
 * @author HenryAWE
 * @brief Input management
 */

#ifndef SROSE_WM_input_hpp_
#define SROSE_WM_input_hpp_

#include <SDL_events.h>
#include <sr/core/macros.h>


namespace srose::wm
{
    class InputManager
    {
    public:
    };

    InputManager* SRSCALL CreateInputManager();
    void SRSCALL DestroyInputManager() noexcept;
    [[nodiscard]]
    InputManager* SRSCALL GetInputManager() noexcept;
} // namespace srose::wm


#endif
