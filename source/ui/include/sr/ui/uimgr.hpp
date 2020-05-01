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
        UIManager(wm::Window& window);

        void Update();

        /**
         * @brief Send quit event to SDL
         */
        void Quit() noexcept;

        void InitializeWidgets();

        util::string_tree<std::shared_ptr<Widget>> widget_tree;
        std::stack<std::shared_ptr<Widget>> widget_stack;

        /**
         * @brief Pass a new locale to every widget
         * 
         * @param loc Locale object
         */
        void imbue(const std::locale& loc);
    };

    UIManager* SRSCALL CreateUIManager(wm::Window& window);
    void SRSCALL DestroyUIManager() noexcept;
    [[nodiscard]]
    UIManager* SRSCALL GetUIManager() noexcept;
} // namespace srose::ui


#endif
