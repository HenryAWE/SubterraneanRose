/**
 * @file entry.cpp
 * @author HenryAWE
 * @brief Program entry
 */

#include <cassert>
#include <cstdlib>
#include <SDL.h>
#include <typeinfo>
#include <future>
#include <imgui.h>
#include <sr/core/init.h>
#include <sr/ui/entry.h>
#include <sr/ui/console/cmdline.hpp>
#include <sr/filesystem/filesystem.hpp>
#include <sr/wm/winmgr.hpp>
#include <sr/wm/input.hpp>
#include <sr/audio/aumgr.hpp>
#include <sr/res/resmgr.hpp>
#include <sr/ui/gui/uimgr.hpp>
#include <sr/player/player.hpp>
#include "main_loop.hpp"
#include "i18n/i18n.hpp"


static void SRSCALL LoadFonts()
{
    auto& io = ImGui::GetIO();
    auto arialuni = srose::filesystem::GetFont("ARIALUNI");
    if(arialuni)
        io.Fonts->AddFontFromFileTTF(arialuni->string().c_str(), 22.5f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontDefault();
    io.Fonts->Build();
}

static void SRSCALL SetWorkingDirectory(const char* argv0)
{
    namespace fs = srose::filesystem;

    std::error_code ec = {};
    fs::current_path(fs::u8path(argv0).parent_path(), ec);
    if (ec)
    { // Set working directory failed, use default path instead
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[UI] Set program executing path failed, use \"%s\" instead",
            fs::current_path().u8string().c_str()
        );
    }
}

int SRSCALL program_entry(int argc, char* argv[])
{
    using namespace srose;
    using namespace srose::ui;

    SetWorkingDirectory(argv[0]);
    LoadAllLanguage(filesystem::GetLocaleFolder());

    if(SR_UI_CONSOLE_ParseArg(argc, argv) == 1)
    {
        return EXIT_SUCCESS;
    }

    if(SR_CORE_Init(argc, argv, true) != 0)
    { // Init failed
        return EXIT_FAILURE;
    }
    else
    {
        at_quick_exit(&SR_CORE_QuitSDL); // We'll handle deinitialization on normal exit
    }

    int exit_code = EXIT_SUCCESS;
    int window_flags = 0;
    window_flags |= SR_UI_CONSOLE_FullscreenRequired()?SDL_WINDOW_FULLSCREEN:0;
    SR_WM_display* display = SR_WM_CreateDisplay(
        "Subterranean Rose",
        window_flags
    );
    if(!display)
    {
        exit_code = EXIT_FAILURE;
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "SR_WM_CreateDisplay() failed",
            "See log for detailed information",
            NULL
        );

        goto quit_program;
    }

    try
    {
        wm::CreateRenderer(display);
        auto font_ready = std::async(std::launch::async, LoadFonts);
        audio::CreateAudioManager();
        res::CreateResourceManager();
        wm::CreateInputManager();
        SDL_SetWindowTitle(display->win, GetDefaultLanguage()->gettext("srose", "Subterranean Rose").c_str());
        font_ready.get();
        ui::CreateUIManager()->InitializeWidgets();
        BeginMainLoop();
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

        goto quit_program;
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

quit_program:
    player::ReleaseUIData();
    wm::GetRenderer()->ReleaseUIData();
    ui::DestroyUIManager();
    res::DestroyResourceManager();
    audio::DestroyAudioManager();
    wm::DestroyInputManager();
    wm::DestroyRenderer();
    SR_WM_DestroyDisplay(display);
    SR_CORE_QuitSDL();
    return exit_code;
}
