/**
 * @file main.cpp
 * @author HenryAWE
 * @brief Program entry
 */

#include <sr/ui/entry.hpp>
#include <SDL_main.h>
#include <fmt/core.h>
#include "initialize.hpp"
#include <sr/i18n/i18n.hpp>
#include <sr/core/init.hpp>
#include <sr/trace/log.hpp>
#include <sr/trace/trace.hpp>
#include <sr/console/cmdline.hpp>
#include <sr/filesystem/filesystem.hpp>
#include <sr/util/shell.hpp>
#include "video.hpp"
#ifdef min
#   undef min
#endif
#ifdef max
#   undef max
#endif

/*Program entry */
int main(int argc, char* argv[])
{
    using namespace srose;

    trace::RegisterSignalHandler();

    SetWorkingDirectory(argv[0]);
    trace::InitializeLogger();
    trace::RedirectSDLOutput();
    i18n::LoadAllLanguage(filesystem::GetLocaleFolder());
    i18n::SelectLanguage();

    auto& cli = progopt::CommandLineInterface::GetGlobalInstance();
    try
    {
        if(cli.ParseArg(argc, argv) == false)
            goto quit_cli;

        cli.HandleArg();
        if(cli.GetBool("lang-available").value_or(false))
        {
            cli.WinRequestConsole(true);

            auto& os = cli.GetOutputStream();
            const auto& lang_set = i18n::GetLanguageSet();
            const auto builtin = i18n::GetBuiltinLanguage();
            const std::string lcfmt = "{}\t: {}";

            os << SRTR("srose.cli.lang.available.built-in") << std::endl;
            os << fmt::format(lcfmt, builtin->GetId(), builtin->GetName()) << std::endl;

            os << SRTR("srose.cli.lang.available.installed") << std::endl;
            for(auto& i : lang_set)
            {
                if(i == builtin)
                    continue;
                os << fmt::format(lcfmt, i->GetId(), i->GetName()) << std::endl;
            }

            cli.WinRequestPause();
            cli.RequestQuit();
        }
        if(cli.Exists("video-get-display-mode"))
        {
            auto disp_index = cli.GetInt("video-get-display-mode");
            if(!disp_index)
                goto quit_cli;

            cli.WinRequestConsole(true);
            if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
            {
                cli.GetOutputStream()
                    << "Initialize SDL VIDEO subsystem failed: "
                    << SDL_GetError()
                    << std::endl;
            }
            else
            {
                try
                {
                    Video_GetDisplayMode(std::max(*disp_index, 0), cli.GetOutputStream());
                }
                catch(...)
                {
                    SDL_QuitSubSystem(SDL_INIT_VIDEO);
                    throw;
                }
                SDL_QuitSubSystem(SDL_INIT_VIDEO);
            }
            cli.WinRequestPause();
            cli.RequestQuit();
        }
        if(cli.GetBool("print-appdata").value_or(false))
        {
            cli.WinRequestConsole(true);
            cli.GetOutputStream() << filesystem::GetAppData().u8string() << std::endl;
            cli.WinRequestPause();
            cli.RequestQuit();
        }
        if(cli.GetBool("explore-appdata").value_or(false))
        {
            util::OpenInBrowser(filesystem::GetAppData().u8string().c_str());
            cli.RequestQuit();
        }

    quit_cli:
        if(cli.QuitRequested())
        {
            #ifdef _WIN32
            if(cli.WinPauseRequested())
            {
                cli.WinRequestConsole(true);
                std::system("pause");
            }
            #endif
            return EXIT_SUCCESS;
        }
    }
    catch(const std::exception& e)
    {
        BOOST_LOG_TRIVIAL(error)
            << fmt::format("Exception ({}) thrown by CLI: {}", typeid(e).name(), e.what());
        return 3;
    }

    cli.WinRequestConsole();
    if(core::Init(argc, argv, true) != 0)
    { // Init failed
        return EXIT_FAILURE;
    }
    else
    {
        // We'll handle deinitialization on normal exit
        at_quick_exit(&core::Quit);
    }

    int window_flags = 0;
    window_flags |= cli.FullscreenRequired()?SDL_WINDOW_FULLSCREEN:0;
    wm::Window window;
    try
    {
        window.Create(
            {1280, 960},
            "Subterranean Rose",
            0,
            window_flags
        );
        if(cli.VSyncRequired())
            window.SetVSync(true);
    }
    catch(const std::runtime_error& e)
    { // Create display failed
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Failed to create window",
            (std::string(e.what()) + "\nSee log for detailed information").c_str(),
            nullptr
        );

        core::Quit();
        return EXIT_FAILURE;
    }

    InitializeAllSystems(window);

    int exit_code = srose::ProgramEntry(window);

    DeinitializeAllSystems(window);
    window.Destroy();
    core::Quit();
    return exit_code;
}
