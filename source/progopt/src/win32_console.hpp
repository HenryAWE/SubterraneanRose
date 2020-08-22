/**
 * @file win32_console.hpp
 * @author HenryAWE
 * @brief Console support for Windows
 */

#ifndef SROSE_PROGOPT_win32_console_hpp_
#define SROSE_PROGOPT_win32_console_hpp_

#include <SDL_platform.h>
#if defined(__WINDOWS__) && !defined(SROSE_DISABLE_CLI)

namespace srose::progopt
{
    bool AttachConsoleWin32(bool& vt) noexcept;
    bool AllocConsoleWin32(bool& vt) noexcept;

    void ReleaseConsoleWin32() noexcept;
} // namespace srose::progopt

#endif

#endif
