/**
 * @file entry.cpp
 * @author HenryAWE
 * @brief Program entry
 */

#include <sr/ui/entry.hpp>
#include <cassert>
#include <cstdlib>
#include <SDL.h>
#include <typeinfo>
#include <boost/locale.hpp>
#ifdef __WINDOWS__
#   include <SDL_syswm.h>
#endif
#include "main_loop.hpp"


namespace srose
{
    int SRSCALL ProgramEntry(wm::Window& window) noexcept
    {
        using namespace srose;

        int exit_code = EXIT_SUCCESS;
        try
        {
            exit_code = BeginMainLoop(window);
        }
        #ifdef __WINDOWS__
        catch(std::system_error& ex)
        {
            CPINFOEXA info;
            ::GetCPInfoExA(CP_OEMCP, 0, &info);

            std::string what = boost::locale::conv::to_utf<char>(
                ex.what(),
                "cp" + std::to_string(info.CodePage)
            );
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[UI] System error \"%s\": %s",
                typeid(ex).name(),
                what.c_str()
            );
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                typeid(ex).name(),
                what.c_str(),
                nullptr
            );
        }
        #endif
        catch(std::exception& ex)
        {
            exit_code = EXIT_FAILURE;
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[UI] Caught exception \"%s\": %s",
                typeid(ex).name(),
                ex.what()
            );
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                typeid(ex).name(),
                ex.what(),
                nullptr
            );
        }
        catch(...)
        {
            exit_code = EXIT_FAILURE;
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[UI] Caught unknown exception"
            );

            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "UNCAUGHT EXCEPTION",
                "Unknown exception",
                nullptr
            );
        }

        return exit_code;
    }
} // namespace srose
