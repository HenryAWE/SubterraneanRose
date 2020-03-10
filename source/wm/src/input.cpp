/**
 * @file input.cpp
 * @author HenryAWE
 * @brief Input management
 */

#include <sr/wm/input.hpp>
#include <memory>


namespace srose::wm
{
    static std::unique_ptr<InputManager> g_input;

    InputManager* SRSCALL CreateInputManager()
    {
        g_input = std::make_unique<InputManager>();
        return g_input.get();
    }
    void SRSCALL DestroyInputManager() noexcept
    {
        g_input.reset();
    }
    [[nodiscard]]
    InputManager* SRSCALL GetInputManager() noexcept
    {
        return g_input.get();
    }
} // namespace srose::wm
