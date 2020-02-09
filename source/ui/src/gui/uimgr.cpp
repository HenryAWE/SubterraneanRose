/**
 * @file uimgr.cpp
 * @author HenryAWE
 * @brief UI Manager
 */

#include <sr/ui/gui/uimgr.hpp>
#include <SDL_events.h>
#include <memory>
#include "mainmenu.hpp"
#include "editor/editor_window.hpp"
#include "configmenu.hpp"


namespace srose::ui
{
    void UIManager::Update()
    {
        if(!widget_stack.empty())
        {
            widget_stack.top()->Update();
        }
    }

    void UIManager::Quit() noexcept
    {
        SDL_Event quit_event{};
        quit_event.type = SDL_QUIT;
        SDL_PushEvent(&quit_event);
    }

    void UIManager::InitializeWidgets()
    {
        widget_stack.push(std::make_shared<MainMenu>());
        widget_tree.emplace_at("mainmenu", widget_stack.top());
        widget_tree.emplace_at("editor.window", std::make_shared<editor::EditorWindow>());
        widget_tree.emplace_at("configpanel", std::make_shared<ConfigPanel>());
    }

    static std::unique_ptr<UIManager> g_uimgr;

    UIManager* SRSCALL CreateUIManager()
    {
        g_uimgr = std::make_unique<UIManager>();
        return g_uimgr.get();
    }
    void SRSCALL DestroyUIManager() noexcept
    {
        g_uimgr.reset();
    }

    UIManager* SRSCALL GetUIManager() noexcept
    {
        return g_uimgr.get();
    }
} // namespace srose::ui
