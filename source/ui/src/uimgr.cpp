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
        if(!m_ui_node_stack.empty())
            m_ui_node_stack.back()->Update();

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

        auto root = m_ui_node_tree.emplace_at("root", std::make_shared<MainMenu>());
        PushRootNode(std::move(root));
    }
    void UIManager::Deinitialize() noexcept
    {
        m_ui_node_stack.clear();
        m_ui_node_tree.clear();
        m_window = nullptr;
    }

    void UIManager::PushRootNode(std::shared_ptr<RootNode> node)
    {
        assert(node);
        m_ui_node_stack.emplace_back(std::move(node));
    }
    void UIManager::PopRootNode()
    {
        if(!m_ui_node_stack.empty())
            m_ui_node_stack.pop_back();
    }
    void UIManager::ResetRootNode(std::shared_ptr<RootNode> node)
    {
        m_ui_node_stack.clear();
        m_ui_node_stack.emplace_back(std::move(node));
    }
    RootNode* UIManager::GetUINodeStackTop() noexcept
    {
        return m_ui_node_stack.empty() ? nullptr : m_ui_node_stack.back().get();
    }

    wm::Window& UIManager::GetWindow() noexcept
    {
        assert(m_window);
        return *m_window;
    }

    void UIManager::imbue(const std::locale& loc)
    {
        OnImbue(loc);
    }
} // namespace srose::ui
