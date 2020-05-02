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
#include <imgui.h>
#include <imguisr.h>
#include <sr/core/macros.hpp>
#include <sr/wm/window.hpp>
#include <sr/filesystem/common.hpp>
#include <sr/util/string_tree.hpp>
#include "widget.hpp"


namespace srose::ui
{
    class UIManager
    {
        wm::Window* m_window = nullptr;
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

        util::string_tree<std::shared_ptr<Widget>> widget_tree;
        std::stack<std::shared_ptr<Widget>> widget_stack;

        /**
         * @brief Pass a new locale to every widget
         * 
         * @param loc Locale object
         */
        void imbue(const std::locale& loc);
    };
} // namespace srose::ui


#endif
