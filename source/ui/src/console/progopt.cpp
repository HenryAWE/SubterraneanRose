/**
 * @file progopt.cpp
 * @author HenryAWE
 * @brief Program options
 */

#if !defined SROSE_DISABLE_CUI // Controlled by the build flag SROSE_DISABLE_CUI

#include <sr/ui/console/progopt.hpp>
#include <boost/program_options.hpp>
#include <memory>
#include <sstream>
#include <cstdio>
#include <cassert>
#include <sr/core/version_info.hpp>
#include <SDL.h>
#include <sr/ui/console/cmdline.hpp>
#include "mswin.hpp"
#include "../i18n/i18n.hpp"

#ifdef _
#   undef _
#endif
#define _(path) (SRTR(path).str())

namespace srose::ui::console
{
    /*Global variables map */
    static std::unique_ptr<boost::program_options::variables_map> vm;

    const boost::program_options::variables_map& GetVariablesMapInternal() noexcept
    {
        assert(vm&&"Call SR_UI_CONSOLE_ParseArg() first!!!");
        return *vm.get();
    }

    static boost::program_options::options_description SRSCALL BuildDescription()
    {
        using namespace srose;
        namespace po = boost::program_options;

        po::options_description generic(_("srose.cui.generic"));
        generic.add_options()
            ("help", _("srose.cui.generic.help").c_str())
            ("version", _("srose.cui.generic.version").c_str())
            ("build-info", _("srose.cui.generic.build").c_str());

        po::options_description language(_("srose.cui.lang"));
        language.add_options()
            ("language", po::value<std::string>()->value_name("name")->default_value("auto"), _("srose.cui.lang.language").c_str())
            ("available-language", _("srose.cui.lang.available").c_str());

        po::options_description display(_("srose.cui.display"));
        display.add_options()
            ("fullscreen,F", _("srose.cui.display.fullscreen").c_str());
        
        po::options_description video(_("srose.cui.video"));
        video.add_options()
            ("get-display-mode", po::value<int>()->value_name("index")->implicit_value(0), _("srose.cui.get-display-mode").c_str());

    #ifdef __WINDOWS__
        po::options_description win32("Windows");
        win32.add_options()
            ("win-console", po::value<std::string>()->value_name("mode")->default_value("auto"), "Windows console");
    #endif

        return po::options_description(_("srose.cui.total"))
            .add(generic)
        #ifdef __WINDOWS__
            .add(win32)
        #endif
            .add(language)
            .add(display)
            .add(video);
    }

    bool GetDisplayMode();

    int SRSCALL ParseArg(int argc, char* argv[]) noexcept
    {
        try
        {
            using namespace srose;
            namespace po = boost::program_options;

            std::ostream& os = std::cout;

            if(!vm)
                vm = std::make_unique<po::variables_map>();

            ui::SelectLanguage();
            auto desc = BuildDescription();
            po::store(po::parse_command_line(argc, argv, desc), *vm);
            po::notify(*vm);

            std::string preferred = ui::console::GetPreferredLanguage();
            ui::SelectLanguage(preferred.empty() ? nullptr : preferred.c_str());

        #ifdef __WINDOWS__
            struct HelperWin32
            {
                bool release = false;
                bool alloc = false;
                HelperWin32() noexcept = default;
                ~HelperWin32() noexcept
                {
                    if(release) ReleaseConsoleWin32();
                }
            };
            static HelperWin32 helper_win32;

            std::string conmode_win32 = (*vm)["win-console"].as<std::string>();
            if(conmode_win32 == "new")
                helper_win32.alloc = helper_win32.release = AllocConsoleWin32();
            if(conmode_win32 != "ignore")
                helper_win32.release = AttachConsoleWin32();

            #define SR_PAUSE_IF_NECESSARY() do{ if(helper_win32.alloc) std::system("PAUSE"); }while(0)
        #else
            #define SR_PAUSE_IF_NECESSARY() do{}while(0)
        #endif

            if(vm->count("help"))
            {
                std::stringstream ss;
                if(preferred.empty())
                    ss << desc;
                else
                    ss << BuildDescription();

                std::printf(ss.str().c_str());

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }
            if(vm->count("version"))
            {
                std::printf(
                    "%d.%d.%d - %s\n",
                    SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH,
                    core::GitCommitShortID()
                );

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }
            if(vm->count("build-info"))
            {
                os
                    << "Subterranean Rose " << core::GetVersionString() << std::endl
                    << core::GitCommitID() << " - "
                    << core::GitCommitMsg() << std::endl;

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }
            if(vm->count("available-language"))
            {
                auto& lm = ui::GetLanguageMap();
                std::stringstream ss;
                ss << _("srose.cui.lang.available.msg") << std::endl;
                for(auto& i : lm)
                {
                    ss
                        << i.second->iso()
                        << "\t: "
                        << i.second->name()
                        << std::endl;
                }

                std::printf(ss.str().c_str());

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }
            if(vm->count("get-display-mode"))
            {
                if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
                {
                    std::printf(
                        "Initialize SDL VIDEO subsystem failed: %s",
                        SDL_GetError()
                    );
                    SR_PAUSE_IF_NECESSARY();
                    return 1;
                }

                GetDisplayMode();

                SDL_QuitSubSystem(SDL_INIT_VIDEO);
                SR_PAUSE_IF_NECESSARY();
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

    bool GetDisplayMode()
    {
        int disp_index = 0;
        try{ disp_index = (*vm)["get-display-mode"].as<int>(); }catch(...){}
        int dm_count = SDL_GetNumDisplayModes(disp_index);
        if(dm_count < 1)
        {
            std::printf(
                "Get number of the display modes of the display-%02d failed: %s\n",
                disp_index,
                SDL_GetError()
            );
            return false;
        }

        const char* disp_name = SDL_GetDisplayName(disp_index);
        std::printf(_("srose.cui.get-display-mode.display-name").c_str(), disp_index, disp_name?disp_name:"(failed)");
        std::printf("\n");
        std::printf(_("srose.cui.get-display-mode.display-mode-count").c_str(), dm_count);
        std::printf("\n");
        std::printf(_("srose.cui.get-display-mode.format-desc").c_str());
        std::printf("\n");

        auto fmt = _("srose.cui.get-display-mode.format") + '\n';
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
                fmt.c_str(),
                i,
                SDL_BITSPERPIXEL(mode.format),
                SDL_GetPixelFormatName(mode.format) + sizeof("SDL_PIXELFORMAT"),
                mode.w, mode.h,
                mode.refresh_rate
            );
        }

        return true;
    }


    int SRSCALL FullscreenRequired() noexcept
    {
        assert(vm&&"Call ParseArg() first!!!");
        return (bool)vm->count("fullscreen");
    }
    int SRSCALL DrawDebugOverlay() noexcept
    {
        assert(vm&&"Call ParseArg() first!!!");
        return (bool)vm->count("draw-debug-overlay");
    }
} // namespace srose::ui::console

#else
/*Dummy implementation for disabled CUI module */
#pragma message("Use dummy implementation for CUI")
#include <sr/ui/console/progopt.h>
#include <cstdio>
#include "../i18n/i18n.hpp"

namespace srose::ui::console
{
    int SRSCALL ParseArg(int argc, char* argv[]) noexcept
    {
        if(argc > 1)
        {
            std::printf(
                "The program was built with SROSE_DISABLE_CUI set to ON\n"
                "If you want the functionality of the command line UI, "
                "please re-build the program with SROSE_DISABLE_CUI set to OFF\n"
            );
        }

        srose::ui::SelectLanguage();
        return 0;
    }

    int SRSCALL FullscreenRequired() noexcept { return false; }
    int SRSCALL DrawDebugOverlay() noexcept { return false; }
} // namespace srose::ui::console

#endif
