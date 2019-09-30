/**
 * @file progopt.cpp
 * @author HenryAWE
 * @brief Program options
 */

#if defined SR_ENABLE_CUI // Controlled by the build flag SROSE_ENABLE_CUI

#include <sr/ui/console/progopt.h>
#include <boost/program_options.hpp>
#include <memory>
#include <sstream>
#include <cstdio>
#include <cassert>
#include <sr/core/version_info.h>
#include <SDL.h>


/*Global variables map */
static std::unique_ptr<boost::program_options::variables_map> vm;

int SRSCALL SR_UI_CONSOLE_ParseArg(int argc, char* argv[])
{
    try
    {
        namespace po = boost::program_options;

        if(!vm)
            vm = std::make_unique<po::variables_map>();

        po::options_description generic("Generic");
        generic.add_options()
            ("help", "This message")
            ("version", "Version information");

        po::options_description display("Display");
        display.add_options()
            ("fullscreen,F", "Fullscreen");
        
        po::options_description video("Video");
        video.add_options()
            ("get-display-mode", po::value<int>()->value_name("index")->implicit_value(0), "Get all available display mode(s) of the specific display");

        po::options_description total("Allowed options");
        total
            .add(generic)
            .add(display)
            .add(video);

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

#else
/*Dummy implementation for disabled CUI module */
#pragma message("Use dummy implementation for CUI")
#include <sr/core/macros.h>
#include <cstdio>


int SRSCALL SR_UI_CONSOLE_ParseArg(int argc, char* argv[])
{
    if(argc > 1)
    {
        std::printf(
            "The program was built with SROSE_ENABLE_CUI set to OFF\n"
            "If you want the functionality of the command line UI, "
            "please re-build the program with SROSE_ENABLE_CUI set to ON\n"
        );
    }

    return 0;
}

int SRSCALL SR_UI_CONSOLE_FullscreenRequired() { return false; }

#endif
