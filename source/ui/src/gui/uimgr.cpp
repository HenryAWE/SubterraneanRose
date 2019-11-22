/**
 * @file uimgr.cpp
 * @author HenryAWE
 * @brief UI Manager
 */

#include "uimgr.hpp"
#include <memory>


namespace srose::ui
{
    void UIManager::Update()
    {

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
