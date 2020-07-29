/**
 * @file mswin.cpp
 * @author HenryAWE
 * @brief Console support for Windows
 */

#include <SDL_platform.h>
#include "mswin.hpp"
#include <stdio.h>
#ifdef __WINDOWS__
#   include <Windows.h>
#endif


#if defined(__WINDOWS__) && !defined(SROSE_DISABLE_CLI)

namespace srose::console
{
    ::BOOL g_attached = FALSE;

    static void InitializeConsoleWin32() noexcept
    {
        ::SetConsoleOutputCP(CP_UTF8);
        ::HANDLE console = ::GetStdHandle(STD_OUTPUT_HANDLE);
        ::DWORD mode = 0;
        ::GetConsoleMode(console, &mode);
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        ::SetConsoleMode(console, mode);
    }
    static void RedirectIOWin32() noexcept
    {
        FILE* dummy = nullptr;
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

    bool AttachConsoleWin32() noexcept
    {
        g_attached = ::AttachConsole(ATTACH_PARENT_PROCESS);
        if(g_attached)
        {
            InitializeConsoleWin32();
            RedirectIOWin32();
            ::SetConsoleOutputCP(CP_UTF8);
            ::WriteConsoleA(::GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, nullptr, nullptr);
        }

        return g_attached;
    }
    bool AllocConsoleWin32() noexcept
    {
        g_attached = ::AllocConsole();
        if(g_attached)
        {
            InitializeConsoleWin32();
            RedirectIOWin32();
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
} // namespace srose::console

#endif
