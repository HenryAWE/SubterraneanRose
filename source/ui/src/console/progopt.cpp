/**
 * @file progopt.cpp
 * @author HenryAWE
 * @brief Program options
 */

#if !defined SROSE_DISABLE_CUI // Controlled by the build flag SROSE_DISABLE_CUI

#include <sr/ui/console/progopt.h>
#include <boost/program_options.hpp>
#include <memory>
#include <sstream>
#include <cstdio>
#include <cassert>
#include <sr/core/version_info.h>
#include <SDL.h>
#include "../i18n/i18n.hpp"

#ifdef _
#   undef _
#endif
#define _(path) (SRTR(path).str())

/*Global variables map */
static std::unique_ptr<boost::program_options::variables_map> vm;

extern "C"
const boost::program_options::variables_map& GetVariablesMapInternal() noexcept
{
    assert(vm&&"Call SR_UI_CONSOLE_ParseArg() first!!!");
    return *vm.get();
}


int SRSCALL SR_UI_CONSOLE_ParseArg(int argc, char* argv[])
{
    try
    {
        using namespace srose;
        namespace po = boost::program_options;

        if(!vm)
            vm = std::make_unique<po::variables_map>();

        ui::SelectLanguage();
        po::options_description generic(_("srose.cui.generic"));
        generic.add_options()
            ("help", _("srose.cui.generic.help").c_str())
            ("version", _("srose.cui.generic.version").c_str());

        po::options_description display(_("srose.cui.display"));
        display.add_options()
            ("language", po::value<std::string>()->value_name("name")->default_value("auto"), _("srose.cui.display.language").c_str())
            ("fullscreen,F", _("srose.cui.display.fullscreen").c_str());
        
        po::options_description video(_("srose.cui.video"));
        video.add_options()
            ("get-display-mode", po::value<int>()->value_name("index")->implicit_value(0), _("srose.cui.get-display-mode").c_str());

        po::options_description debug("Debug");
        debug.add_options()
            ("draw-debug-overlay", "Draw overlay debug UI");

        po::options_description total(_("srose.cui.total"));
        total
            .add(generic)
            .add(display)
            .add(video)
            .add(debug);

        po::store(po::parse_command_line(argc, argv, total), *vm);
        po::notify(*vm);

        if(vm->count("help"))
        {
            std::stringstream ss;
            ss << total;
            std::printf(ss.str().c_str());

            return 1;
        }
        if(vm->count("version"))
        {
            std::printf(
                "%d.%d.%d - %s\n",
                SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH,
                SR_CORE_GitCommitShortID()
            );

            return 1;
        }
        if(vm->count("get-display-mode"))
        {
            int disp_index = 0;
            try{ disp_index = (*vm)["get-display-mode"].as<int>(); }catch(...){}

            if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
            {
                std::printf(
                    "Initialize SDL VIDEO subsystem failed: %s",
                    SDL_GetError()
                );
                return 1;
            }

            int dm_count = SDL_GetNumDisplayModes(disp_index);
            if(dm_count < 1)
            {
                std::printf(
                    "Get number of the display modes of the display-%02d failed: %s\n",
                    disp_index,
                    SDL_GetError()
                );
                return 1;
            }

            const char* disp_name = SDL_GetDisplayName(disp_index);
            std::printf("Display-%02d Name: %s\n", disp_index, disp_name?disp_name:"(failed)");
            std::printf("Display mode count: %d\n", dm_count);
            std::printf("Mode\tBits per Pixel - Format Name\t\tWidth x Height @Refresh Rate\n");
            for(int i = 0; i < dm_count; ++i)
            {
                SDL_DisplayMode mode{};
                if(SDL_GetDisplayMode(disp_index, i, &mode) != 0)
                {
                    std::printf(
                        "%03d:\tfailed - %s\n",
                        i,
                        SDL_GetError()
                    );
                    continue;
                }

                std::printf(
                    "%03d:\t%ibpp - %s\t\t%i x %i @%dHz\n",
                    i,
                    SDL_BITSPERPIXEL(mode.format), SDL_GetPixelFormatName(mode.format),
                    mode.w, mode.h,
                    mode.refresh_rate
                );
            }

            SDL_QuitSubSystem(SDL_INIT_VIDEO);
            return 1;
        }

        return 0;
    }
    catch(const std::exception& e)
    {
        std::printf("%s\n", e.what());
        return 1;
    }
    catch(...)
    { // Continue if caught an unknown exception
        std::printf("An unknown exception is caught in SR_UI_CONSOLE_ParseArg()\n");
        return 1;
    }
}


int SRSCALL SR_UI_CONSOLE_FullscreenRequired()
{
    assert(vm&&"Call SR_UI_CONSOLE_ParseArg() first!!!");
    return (bool)vm->count("fullscreen");
}
int SRSCALL SR_UI_CONSOLE_DrawDebugOverlay()
{
    assert(vm&&"Call SR_UI_CONSOLE_ParseArg() first!!!");
    return (bool)vm->count("draw-debug-overlay");
}

#else
/*Dummy implementation for disabled CUI module */
#pragma message("Use dummy implementation for CUI")
#include <sr/ui/console/progopt.h>
#include <cstdio>


int SRSCALL SR_UI_CONSOLE_ParseArg(int argc, char* argv[])
{
    if(argc > 1)
    {
        std::printf(
            "The program was built with SROSE_DISABLE_CUI set to ON\n"
            "If you want the functionality of the command line UI, "
            "please re-build the program with SROSE_DISABLE_CUI set to OFF\n"
        );
    }

    return 0;
}

int SRSCALL SR_UI_CONSOLE_FullscreenRequired() { return false; }
int SRSCALL SR_UI_CONSOLE_DrawDebugOverlay() { return false; }

#endif
