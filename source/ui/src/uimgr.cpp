/**
 * @file uimgr.cpp
 * @author HenryAWE
 * @brief UI Manager
 */

#include <sr/ui/uimgr.hpp>
#include <SDL_events.h>
#include <memory>
#include "gui/mainmenu.hpp"
#include "gui/editor/editor_window.hpp"
#include "gui/configmenu.hpp"
#include "gui/about.hpp"
#include "gui/conwin.hpp"


namespace srose::ui
{
    UIManager::UIManager() = default;

    UIManager::~UIManager() = default;

    UIManager& UIManager::GetInstance()
    {
        static UIManager instance;
        return instance;
    }

    void UIManager::Update()
    {
        assert(root);
        root->Update();

        for(auto& i : m_standalone)
        {
            if(i) i->Update();
        }
    }

    void UIManager::Quit() noexcept
    {
        SDL_Event quit_event{};
        quit_event.type = SDL_QUIT;
        SDL_PushEvent(&quit_event);
    }

    void UIManager::Initialize(wm::Window& window)
    {
        m_window = &window;

        root = std::make_shared<MainMenu>();
    }
    void UIManager::Deinitialize() noexcept
    {
        root.reset();
        m_window = nullptr;
    }

    wm::Window& UIManager::GetWindow() noexcept
    {
        assert(m_window);
        return *m_window;
    }

    std::vector<std::shared_ptr<StandaloneNode>>& UIManager::GetStandaloneVector()
    {
        return m_standalone;
    }

    void UIManager::imbue(const std::locale& loc)
    {
        OnImbue(loc);
    }
} // namespace srose::ui
