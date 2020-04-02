/**
 * @file input.hpp
 * @author HenryAWE
 * @brief Input management
 */

#ifndef SROSE_WM_input_hpp_
#define SROSE_WM_input_hpp_

#include <SDL_events.h>
#include <tuple>
#include <vector>
#include <sr/core/macros.hpp>
#include <sr/wm/event.hpp>


namespace srose::wm
{
    class InputManager
    {
        bool m_loop = true;
    public:
        InputManager();

        void ProcessAllEvent();

        void HandleEvent(SDL_Event& e);

        void NotifyImGui(SDL_Event& e);

        operator bool() const noexcept { return m_loop; }

        enum class Keys
        {
            UP = 0,
            DOWN,
            LEFT,
            RIGHT,
            COUNT
        };

        bool KeyDown(Keys key);

    private:
        std::vector<std::tuple<std::int32_t, std::int32_t>> m_keys;
    };

    InputManager* SRSCALL CreateInputManager();
    void SRSCALL DestroyInputManager() noexcept;
    [[nodiscard]]
    InputManager* SRSCALL GetInputManager() noexcept;
} // namespace srose::wm


#endif
