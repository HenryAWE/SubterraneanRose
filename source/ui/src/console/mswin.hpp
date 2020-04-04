/**
 * @file mswin.hpp
 * @author HenryAWE
 * @brief Console support for Windows
 */

#ifndef SROSE_UI_CONSOLE_mswin_hpp_
#define SROSE_UI_CONSOLE_mswin_hpp_

#include <SDL_platform.h>
#if defined(__WINDOWS__) && !defined(SROSE_DISABLE_CUI)

namespace srose::ui::console
{
    bool AttachConsoleWin32() noexcept;
    bool AllocConsoleWin32() noexcept;

    void ReleaseConsoleWin32() noexcept;
} // namespace srose::ui::console

#endif

#endif
