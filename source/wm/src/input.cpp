/**
 * @file input.cpp
 * @author HenryAWE
 * @brief Input management
 */


#include <sr/wm/input.hpp>
#include <memory>
#include <imgui.h>
#include <imgui_impl_sdl.h>


namespace srose::wm
{
    void InputManager::ProcessAllEvent()
    {
        SDL_Event e{};
        while(SDL_PollEvent(&e))
        {
            NotifyImGui(e);

            HandleEvent(e);
        }
    }
    void InputManager::HandleEvent(SDL_Event& e)
    {
        switch(e.type)
        {
        case SDL_QUIT:
            m_loop = SDL_FALSE;
            break;
        }
    }

    void InputManager::NotifyImGui(SDL_Event& e)
    {
        ImGui_ImplSDL2_ProcessEvent(&e);
    }

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
