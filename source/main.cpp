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

    auto& cli = console::CommandLineInterface::GetGlobalInstance();
    try
    {
        cli.ParseArg(argc, argv);
        cli.HandleArg();
        if(cli.Exists("lang-available"))
        {
            cli.WinRequestOutput(true);

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
            int disp_index = 0;
            try{ disp_index = cli.GetInt("video-get-display-mode"); }catch(...){}

            cli.WinRequestOutput(true);
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
                    Video_GetDisplayMode(std::max(disp_index, 0), cli.GetOutputStream());
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
        if(cli.Exists("print-appdata"))
        {
            cli.WinRequestOutput(true);
            cli.GetOutputStream() << filesystem::GetAppData().u8string() << std::endl;
            cli.WinRequestPause();
            cli.RequestQuit();
        }
        if(cli.Exists("explore-appdata"))
        {
            util::OpenInBrowser(filesystem::GetAppData().u8string().c_str());
            cli.RequestQuit();
        }

        if(cli.QuitRequested())
        {
            #ifdef _WIN32
            if(cli.WinPauseRequested())
            {
                cli.WinRequestOutput(true);
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

    cli.WinRequestOutput();
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
