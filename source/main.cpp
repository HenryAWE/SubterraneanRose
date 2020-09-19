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
#include "maincli.hpp"


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
        if(cli.HandleArg() != true)
            goto quit_cli;
        HandleConsoleArg(cli);

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
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << "Exception (Unknown) thrown by CLI";
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

    const bool disable_audio = cli.GetBool("audio-disable").value_or(false);
    if(disable_audio)
    {
        cli.OutputWarning("WARNING: Audio system has been disabled!\n");
        BOOST_LOG_TRIVIAL(info)
            << "Audio system has been disabled";
    }
    InitializeAllSystems(window, disable_audio);

    int exit_code = srose::ProgramEntry(window);

    DeinitializeAllSystems(window, disable_audio);
    window.Destroy();
    core::Quit();
    return exit_code;
}
