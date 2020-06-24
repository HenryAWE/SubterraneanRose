/**
 * @file uimgr.hpp
 * @author HenryAWE
 * @brief UI Manager
 */

#ifndef SROSE_UI_uimgr_hpp_
#define SROSE_UI_uimgr_hpp_

#include <stack>
#include <functional>
#include <optional>
#include <boost/signals2.hpp>
#include <imgui.h>
#include <imguisr.h>
#include <sr/core/macros.hpp>
#include <sr/wm/window.hpp>
#include <sr/filesystem/common.hpp>
#include <sr/util/string_tree.hpp>
#include "widget.hpp"
#include "node.hpp"


namespace srose::ui
{
    class UIManager
    {
        wm::Window* m_window = nullptr;

        std::vector<std::shared_ptr<StandaloneNode>> m_standalone;
        util::string_tree<std::shared_ptr<RootNode>> m_ui_node_tree;
        std::vector<std::shared_ptr<RootNode>> m_ui_node_stack;
    public:
        UIManager();

        ~UIManager();

        [[nodiscard]]
        static UIManager& GetInstance();

        void Update();

        /**
         * @brief Send quit event to SDL
         */
        void Quit() noexcept;

        void Initialize(wm::Window& window);
        void Deinitialize() noexcept;

        wm::Window& GetWindow() noexcept;

        [[nodiscard]]
        constexpr std::vector<std::shared_ptr<StandaloneNode>>& GetStandaloneNodes() { return m_standalone; }
        [[nodiscard]]
        constexpr util::string_tree<std::shared_ptr<RootNode>>& GetUINodeTree() { return m_ui_node_tree; }

        void PushRootNode(std::shared_ptr<RootNode> node);
        void PopRootNode();
        void ResetRootNode(std::shared_ptr<RootNode> node);
        [[nodiscard]]
        RootNode* GetUINodeStackTop() noexcept;

        util::string_tree<std::shared_ptr<Widget>> widget_tree;
        std::stack<std::shared_ptr<Widget>> widget_stack;

        boost::signals2::signal<void(const std::locale&)> OnImbue;

        /**
         * @brief Pass a new locale to every widget
         * 
         * @param loc Locale object
         */
        void imbue(const std::locale& loc);
    };
} // namespace srose::ui


#endif
