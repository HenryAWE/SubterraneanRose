/**
 * @file shell.cpp
 * @author HenryAWE
 * @brief Shell API abstraction
 */

#include <sr/util/shell.hpp>
#include <SDL_platform.h>
#include <SDL_syswm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#ifdef __WINDOWS__
#   include <wchar.h>
#   include <shellapi.h>
#elif defined __LINUX__
#   include <spawn.h>
#   include <vector>
#   include <boost/tokenizer.hpp>
#   include <sr/filesystem/filesystem.hpp>
#endif


#ifdef __LINUX__
extern "C"
{
    extern char** environ;
}
#endif

namespace srose::util
{
    void SRSCALL OpenInBrowser(const std::string& url) noexcept
    try
    {
    #ifdef __WINDOWS__
        /*Convert URL to wide char to avoid encoding issue */
        std::size_t wurl_len = MultiByteToWideChar(CP_UTF8, 0 , url.c_str(), (int)url.length(), nullptr, 0);
        std::wstring wurl(wurl_len, L'\0');
        if(MultiByteToWideChar(CP_UTF8, 0, url.c_str(), (int)url.length(), wurl.data(), (int)wurl.size()) <= 0)
        { // Error
            return;
        }

        ShellExecuteW(
            NULL,
            L"open",
            wurl.c_str(),
            NULL,
            NULL,
            SW_SHOWNORMAL
        );

    #elif defined __LINUX__
        using namespace boost;
        namespace fs = filesystem;

        const char* sys_path = getenv("PATH");
        if(!sys_path)
            return;
        char_separator<char> sep(":");
        tokenizer<char_separator<char>> tok(std::string(sys_path), sep);
        fs::path xdg_open_path;
        for(const auto& i : tok)
        {
            auto tmp_path = fs::path(i) / "xdg-open";
            if(fs::exists(tmp_path) && fs::is_regular_file(tmp_path))
                xdg_open_path.swap(tmp_path);
        }
        if(xdg_open_path.empty())
            return; // xdg-open not found

        // Invoke xdg-open
        pid_t pid{};
        auto xdg_open_str = xdg_open_path.u8string();
        std::vector<char> buffer(xdg_open_str.length() + 1 + url.length() + 1);
        char* xdg_oepn_argv0_pos = buffer.data();
        char* xdg_oepn_argv1_pos = buffer.data() + xdg_open_str.length() + 1;
        memcpy(xdg_oepn_argv0_pos, xdg_open_str.c_str(), xdg_open_str.length());
        memcpy(xdg_oepn_argv1_pos, url.c_str(), url.length());
        char* xdg_open_argv[3] = { xdg_oepn_argv0_pos, xdg_oepn_argv1_pos, nullptr };
        posix_spawn(&pid, buffer.data(), nullptr, nullptr, xdg_open_argv, environ);

    #else
    #   pragma message("Use dummy implementation for OpenInBrowser()")
    #endif
    }
    catch(...) {}
} // namespace srose::util
