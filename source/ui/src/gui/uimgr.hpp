/**
 * @file uimgr.hpp
 * @author HenryAWE
 * @brief UI Manager
 */

#ifndef SROSE_UI_GUI_uimgr_hpp_
#define SROSE_UI_GUI_uimgr_hpp_

#include <stack>
#include <functional>
#include <optional>
#include <imgui.h>
#include <imguisr.h>
#include <sr/core/macros.h>
#include <sr/filesystem/common.hpp>
#include <sr/util/string_tree.hpp>
#include "widget.hpp"


namespace srose::ui
{
    class UIManager
    {
    public:
        void Update();

        /**
         * @brief Send quit event to SDL
         */
        void Quit() noexcept;

        void InitializeWidgets();

        util::string_tree<std::shared_ptr<Widget>> widget_tree;
        std::stack<std::shared_ptr<Widget>> widget_stack;
    };

    UIManager* SRSCALL CreateUIManager();
    void SRSCALL DestroyUIManager() noexcept;
    [[nodiscard]]
    UIManager* SRSCALL GetUIManager() noexcept;
} // namespace srose::ui


#endif
