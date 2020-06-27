/**
 * @file progopt.cpp
 * @author HenryAWE
 * @brief Program options
 */

#if !defined SROSE_DISABLE_CUI // Controlled by the build flag SROSE_DISABLE_CUI

#include <sr/console/progopt.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <memory>
#include <cassert>
#include <sr/core/version_info.hpp>
#include <SDL.h>
#include <sr/console/cmdline.hpp>
#include <sr/i18n/i18n.hpp>
#include "mswin.hpp"
#include "buildinfo.hpp"

#ifdef _
#   undef _
#endif
#define _(path) (SRTR(path).str())

namespace srose::console
{
    /*Global variables map */
    static std::unique_ptr<boost::program_options::variables_map> vm;

    const boost::program_options::variables_map& GetVariablesMapInternal() noexcept
    {
        assert(vm&&"Call SR_UI_CONSOLE_ParseArg() first!!!");
        return *vm.get();
    }

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
#endif

    static void SRSCALL RequestCommandLineOutput(boost::program_options::variables_map& vm, bool force = false)
    {
    #ifdef __WINDOWS__
        if(helper_win32.release == false)
        {
            std::string conmode_win32;
            try { conmode_win32 = vm["win-console"].as<std::string>(); }
            catch(const boost::bad_any_cast&) { conmode_win32 = "auto"; }

            if(conmode_win32 == "new")
            {
                helper_win32.alloc = helper_win32.release = AllocConsoleWin32();
            }
            else if(conmode_win32 == "attach")
            {
                helper_win32.release = AttachConsoleWin32();
            }
            else if(conmode_win32 == "auto")
            {
                helper_win32.release = AttachConsoleWin32();
                if(!helper_win32.release && force)
                    helper_win32.alloc = helper_win32.release = AllocConsoleWin32();
            }
        }
    #endif
    }

    static boost::program_options::options_description SRSCALL BuildDescription()
    {
        using namespace srose;
        namespace po = boost::program_options;

        constexpr unsigned int line_length = UINT_MAX;

        po::options_description generic(_("srose.cui.generic"), line_length);
        generic.add_options()
            ("help", _("srose.cui.generic.help").c_str())
            ("version", _("srose.cui.generic.version").c_str())
            ("build-info", _("srose.cui.generic.build").c_str());

        po::options_description language(_("srose.cui.lang"), line_length);
        language.add_options()
            ("language", po::value<std::string>()->value_name("name")->default_value("auto"), _("srose.cui.lang.language").c_str())
            ("available-language", _("srose.cui.lang.available").c_str());

        po::options_description display(_("srose.cui.display"), line_length);
        display.add_options()
            ("fullscreen,F", _("srose.cui.display.fullscreen").c_str())
            ("vsync,V", "Enable vertical synchronization");
        
        po::options_description video(_("srose.cui.video"), line_length);
        video.add_options()
            ("get-display-mode", po::value<int>()->value_name("index")->implicit_value(0), _("srose.cui.get-display-mode").c_str());

    #ifdef __WINDOWS__
        po::options_description win32("Windows", line_length);
        win32.add_options()
            ("win-console", po::value<std::string>()->value_name("mode")->default_value("auto"), "Windows console");
    #endif

        return po::options_description(_("srose.cui.total"), line_length)
            .add(generic)
        #ifdef __WINDOWS__
            .add(win32)
        #endif
            .add(language)
            .add(display)
            .add(video);
    }

    bool GetDisplayMode(std::ostream& os);

    int SRSCALL ParseArg(int argc, char* argv[]) noexcept
    {
    #ifdef __WINDOWS__
        #define SR_PAUSE_IF_NECESSARY() do{ if(helper_win32.alloc) std::system("PAUSE"); }while(0)
    #else
        #define SR_PAUSE_IF_NECESSARY() do{}while(0)
    #endif
        std::ostream& os = std::cout;
        try
        {
            using namespace srose;
            namespace po = boost::program_options;

            if(!vm)
                vm = std::make_unique<po::variables_map>();

            i18n::SelectLanguage();
            auto desc = BuildDescription();
            po::store(po::parse_command_line(argc, argv, desc), *vm);
            po::notify(*vm);
            RequestCommandLineOutput(*vm);

            std::string preferred = console::GetPreferredLanguage();
            i18n::SelectLanguage(preferred.empty() ? nullptr : preferred.c_str());

            if(vm->count("help"))
            {
                RequestCommandLineOutput(*vm, true);
                std::stringstream ss;
                ss << (preferred.empty() ? desc : BuildDescription());

                os
                    << "Subterranean Rose CUI\n"
                    << ss.str()
                    << std::endl;

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }
            if(vm->count("version"))
            {
                RequestCommandLineOutput(*vm, true);
                os << boost::format("%d.%d.%d - %s")
                        % SR_VERSION_MAJOR % SR_VERSION_MINOR % SR_VERSION_PATCH
                        % core::GitCommitShortID()
                    << std::endl;

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }
            if(vm->count("build-info"))
            {
                RequestCommandLineOutput(*vm, true);
                os
                    << "Subterranean Rose " << core::GetVersionString() << std::endl
                    << core::GitCommitMsg() << " - "
                    << core::GitCommitID() << std::endl;
                const char* commit_body = core::GitCommitBody();
                if(commit_body[0] != '\0')
                {
                    os << std::endl << commit_body << std::endl;
                }
                os << std::endl;

                const char* build_suffix = core::GetBuildSuffix();
                if(build_suffix[0] != '\0')
                {
                    os << "Build suffix: " <<  build_suffix << std::endl;
                }
                os << std::endl;

                /* C++ Information */
                os
                    << "C++ Information" << std::endl
                    << GenerateCppInfo()
                    << std::endl;
                /* Third Party Libraries */
                os
                    << "Third Party Libraries" << std::endl
                    << GenerateLibInfo()
                    << std::endl;

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }
            if(vm->count("available-language"))
            {
                RequestCommandLineOutput(*vm, true);
                auto builtin = i18n::GetBuiltinLanguage();
                os << _("srose.cui.lang.available.built-in") << std::endl;
                os
                    << builtin->iso()
                    << "\t: "
                    << builtin->name()
                    << std::endl;

                auto& lm = i18n::GetLanguageMap();
                os << _("srose.cui.lang.available.installed") << std::endl;
                for(auto& i : lm)
                {
                    if(i.second == builtin)
                        continue;
                    os
                        << i.second->iso()
                        << "\t: "
                        << i.second->name()
                        << std::endl;
                }

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }
            if(vm->count("get-display-mode"))
            {
                RequestCommandLineOutput(*vm, true);
                if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
                {
                    os
                        << "Initialize SDL VIDEO subsystem failed: "
                        << SDL_GetError()
                        << std::endl;

                    SR_PAUSE_IF_NECESSARY();
                    return 1;
                }

                GetDisplayMode(os);
                SDL_QuitSubSystem(SDL_INIT_VIDEO);

                SR_PAUSE_IF_NECESSARY();
                return 1;
            }

            return 0;
        }
        catch(const std::exception& e)
        {
            RequestCommandLineOutput(*vm, true);
            os << e.what() << std::endl;

            SR_PAUSE_IF_NECESSARY();
            return 1;
        }
        catch(...)
        {
            RequestCommandLineOutput(*vm, true);
            os << "An unknown exception is caught in SR_UI_CONSOLE_ParseArg()" << std::endl;

            SR_PAUSE_IF_NECESSARY();
            return 1;
        }
    }

    bool GetDisplayMode(std::ostream& os)
    {
        int disp_index = 0;
        try{ disp_index = (*vm)["get-display-mode"].as<int>(); }catch(...){}
        int dm_count = SDL_GetNumDisplayModes(disp_index);
        if(dm_count < 1)
        {
            os
                << boost::format("Get number of the display modes of the display-%02d failed: %s")
                    % disp_index
                    % SDL_GetError()
                << std::endl;

            return false;
        }

        const char* disp_name = SDL_GetDisplayName(disp_index);

        os
            << boost::format(_("srose.cui.get-display-mode.display-name"))
                % disp_index
                % (disp_name ? disp_name : "(failed)")
            << std::endl
            << boost::format(_("srose.cui.get-display-mode.display-mode-count")) % dm_count
            << std::endl
            << _("srose.cui.get-display-mode.format-desc")
            << std::endl;

        boost::format fmt(_("srose.cui.get-display-mode.format"));
        for(int i = 0; i < dm_count; ++i)
        {
            SDL_DisplayMode mode{};
            if(SDL_GetDisplayMode(disp_index, i, &mode) != 0)
            {
                os
                    << boost::format("%03d:\tfailed - %s\n")
                        % i % SDL_GetError()
                    << std::endl;

                continue;
            }

            os
                << fmt
                    % i
                    % SDL_BITSPERPIXEL(mode.format)
                    % (SDL_GetPixelFormatName(mode.format) + sizeof("SDL_PIXELFORMAT"))
                    % mode.w % mode.h
                    % mode.refresh_rate
                << std::endl;
        }

        return true;
    }


    int SRSCALL FullscreenRequired() noexcept
    {
        assert(vm&&"Call ParseArg() first!!!");
        return (bool)vm->count("fullscreen");
    }
    bool SRSCALL VSyncRequired() noexcept
    {
        assert(vm&&"Call ParseArg() first!!!");
        return vm->count("vsync");
    }
    int SRSCALL DrawDebugOverlay() noexcept
    {
        assert(vm&&"Call ParseArg() first!!!");
        return (bool)vm->count("draw-debug-overlay");
    }
} // namespace srose::console

#else
/*Dummy implementation for disabled CUI module */
#pragma message("Use dummy implementation for CUI")
#include <sr/ui/console/progopt.hpp>
#include <SDL_platform.h>
#include <sr/ui/i18n/i18n.hpp>
#ifdef __WINDOWS__
#   include <Windows.h>
#endif

namespace srose::console
{
    int SRSCALL ParseArg(int argc, char* argv[]) noexcept
    {
        if(argc > 1)
        {
            std::string info = 
                "The program was built with SROSE_DISABLE_CUI set to ON\n"
                "If you want the functionality of the command line UI, "
                "please re-build the program with SROSE_DISABLE_CUI set to OFF";
        #ifdef __WINDOWS__
            ::MessageBox(
                nullptr,
                info.c_str(),
                "The command line UI has been disabled",
                MB_OK | MB_ICONINFORMATION
            );
        #else
            std::cout << info << std::endl;
        #endif
        }

        srose::ui::SelectLanguage();
        return 0;
    }

    int SRSCALL FullscreenRequired() noexcept { return false; }
    bool SRSCALL VSyncRequired() noexcept { return false; }
    int SRSCALL DrawDebugOverlay() noexcept { return false; }
} // namespace srose::console

#endif
