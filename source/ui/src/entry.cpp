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
#include <sr/trace/log.hpp>
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
        catch(const std::system_error& e)
        {
            CPINFOEXA info;
            ::GetCPInfoExA(CP_OEMCP, 0, &info);

            std::string u8what = boost::locale::conv::to_utf<char>(
                e.what(),
                "cp" + std::to_string(info.CodePage)
            );

            BOOST_LOG_TRIVIAL(fatal)
                << "Uncaught exception \"" << typeid(e).name() << "\": "
                << u8what;

            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                typeid(e).name(),
                u8what.c_str(),
                nullptr
            );
        }
        #endif
        catch(const std::exception& e)
        {
            exit_code = EXIT_FAILURE;
            BOOST_LOG_TRIVIAL(fatal)
                << "Uncaught exception \"" << typeid(e).name() << "\": "
                << e.what();
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                typeid(e).name(),
                e.what(),
                nullptr
            );
        }
        catch(...)
        {
            exit_code = EXIT_FAILURE;
            BOOST_LOG_TRIVIAL(fatal)
                << "Uncaught unknown exception";
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
