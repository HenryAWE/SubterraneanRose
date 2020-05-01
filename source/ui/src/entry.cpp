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
