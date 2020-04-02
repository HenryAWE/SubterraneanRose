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
    InputManager::InputManager()
    {
        m_keys.resize(static_cast<std::size_t>(Keys::COUNT));
        #define MAPKEY(idx, k1, k2) \
        do{ m_keys[static_cast<std::size_t>(idx)] = std::make_tuple(k1, k2); }while(0)

        MAPKEY(Keys::UP, SDL_SCANCODE_UP, 0);
        MAPKEY(Keys::DOWN, SDL_SCANCODE_DOWN, 0);
        MAPKEY(Keys::LEFT, SDL_SCANCODE_LEFT, 0);
        MAPKEY(Keys::RIGHT, SDL_SCANCODE_RIGHT, 0);
    }

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

    bool InputManager::KeyDown(Keys key)
    {
        const std::size_t idx = static_cast<std::size_t>(key);
        return
            ImGui::IsKeyDown(std::get<0>(m_keys[idx])) ||
            ImGui::IsKeyDown(std::get<1>(m_keys[idx]));
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
