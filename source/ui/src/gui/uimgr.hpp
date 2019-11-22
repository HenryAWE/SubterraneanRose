/**
 * @file uimgr.hpp
 * @author HenryAWE
 * @brief UI Manager
 */

#ifndef SROSE_UI_GUI_uimgr_hpp_
#define SROSE_UI_GUI_uimgr_hpp_

#include <functional>
#include <optional>
#include <imgui.h>
#include <imguisr.h>
#include <sr/core/macros.h>
#include <sr/filesystem/common.hpp>


namespace srose::ui
{
    class UIManager
    {
    public:
        void Update();
    };

    UIManager* SRSCALL CreateUIManager();
    void SRSCALL DestroyUIManager() noexcept;
    [[nodiscard]]
    UIManager* SRSCALL GetUIManager() noexcept;
} // namespace srose::ui


#endif
