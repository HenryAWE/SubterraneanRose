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
    UIManager::UIManager(wm::Window& window)
        : m_window(&window) {}

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
        widget_tree.emplace_at("configpanel", std::make_shared<ConfigPanel>(*m_window));
        widget_tree.emplace_at("about", std::make_shared<About>());
        widget_tree.emplace_at("conwin", std::make_shared<ConsoleWindow>());
    }

    void imbue_impl(
        util::string_tree<std::shared_ptr<Widget>>& tree,
        const std::locale& loc
    ) {
        auto func = [&loc](auto& w) { w->imbue(loc); };
        for(auto& i : tree)
        {
            i.second.modify(func);
            imbue_impl(i.second, loc);
        }
    }
    void UIManager::imbue(const std::locale& loc)
    {
        imbue_impl(widget_tree, loc);
    }

    static std::unique_ptr<UIManager> g_uimgr;

    UIManager* SRSCALL CreateUIManager(wm::Window& window)
    {
        g_uimgr = std::make_unique<UIManager>(window);
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
