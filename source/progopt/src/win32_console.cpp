/**
 * @file win32_console.cpp
 * @author HenryAWE
 * @brief Console support for Windows
 */

#include <SDL_platform.h>
#include "win32_console.hpp"
#include <stdio.h>
#ifdef __WINDOWS__
#   define WIN32_LEAN_AND_MEAN 1
#   include <Windows.h>
#endif


#if defined(__WINDOWS__) && !defined(SROSE_DISABLE_CLI)

namespace srose::progopt
{
    ::BOOL g_attached = FALSE;

    static void InitializeConsoleWin32(bool& vt, bool input) noexcept
    {
        ::SetConsoleOutputCP(CP_UTF8);
        if(input)
            ::SetConsoleCP(CP_UTF8);
        ::HANDLE console = ::GetStdHandle(STD_OUTPUT_HANDLE);
        ::DWORD mode = 0;
        if(::GetConsoleMode(console, &mode) == 0)
            return;
        if(mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)
        {
            vt = true;
            return;
        }
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if(::SetConsoleMode(console, mode) != 0)
            vt = true;
    }
    static void RedirectIOWin32(bool input) noexcept
    {
        FILE* dummy = nullptr;
        if(input)
            freopen_s(&dummy, "conin$", "r+t", stdin);
        freopen_s(&dummy, "conout$", "w+t", stdout);
        freopen_s(&dummy, "conout$", "w+t", stderr);
    }
    static void ReleaseIOWin32() noexcept
    {
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
    }

    bool AttachConsoleWin32(bool& vt) noexcept
    {
        g_attached = ::AttachConsole(ATTACH_PARENT_PROCESS);
        if(g_attached)
        {
            InitializeConsoleWin32(vt, false);
            RedirectIOWin32(false);
            ::SetConsoleOutputCP(CP_UTF8);
            ::WriteConsoleW(::GetStdHandle(STD_OUTPUT_HANDLE), L"\n", 1, nullptr, nullptr);
        }

        return g_attached;
    }
    bool AllocConsoleWin32(bool& vt) noexcept
    {
        g_attached = ::AllocConsole();
        if(g_attached)
        {
            ::SetConsoleTitleW(L"Subterranean Rose CLI");
            InitializeConsoleWin32(vt, true);
            RedirectIOWin32(true);
        }

        return g_attached;
    }

    void ReleaseConsoleWin32() noexcept
    {
        if(!g_attached)
            return;
        ::FreeConsole();
        ReleaseIOWin32();
        g_attached = FALSE;
    }
} // namespace srose::progopt

#endif
