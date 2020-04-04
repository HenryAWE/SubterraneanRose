/**
 * @file initialize.cpp
 * @author HenryAWE
 * @brief Initializer
 */

#include "initialize.hpp"
#include <future>
#include <SDL_Log.h>
#include <imgui.h>
#include <sr/wm/winmgr.hpp>
#include <sr/audio/aumgr.hpp>
#include <sr/res/resmgr.hpp>
#include <sr/wm/input.hpp>
#include <sr/ui/gui/uimgr.hpp>
#include <sr/player/player.hpp>
#include <sr/srose/app.hpp>


namespace srose
{
    static void LoadFonts()
    {
        auto& io = ImGui::GetIO();
        auto arialuni = srose::filesystem::GetFont("ARIALUNI");
        if(arialuni)
            io.Fonts->AddFontFromFileTTF(arialuni->string().c_str(), 22.5f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        io.Fonts->AddFontDefault();
        io.Fonts->Build();
    }

    void SetWorkingDirectory(const char* argv0)
    {
        namespace fs = srose::filesystem;

        std::error_code ec = {};
        fs::current_path(fs::u8path(argv0).parent_path(), ec);
        if (ec)
        { // Set working directory failed, use default path instead
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[SROSE] Set program executing path failed, use \"%s\" instead",
                fs::current_path().u8string().c_str()
            );
        }
    }

    void InitializeAllSystems(wm::Display* display)
    {
        GetApp().LoadUsers();
        wm::CreateRenderer(display);
        auto font_ready = std::async(std::launch::async, LoadFonts);
        audio::CreateAudioManager();
        res::CreateResourceManager();
        wm::CreateInputManager();
        font_ready.get();
        ui::CreateUIManager()->InitializeWidgets();
    }
    void DeinitializeAllSystems() noexcept
    {
        player::ReleaseUIData();
        wm::GetRenderer()->ReleaseUIData();
        ui::DestroyUIManager();
        res::DestroyResourceManager();
        audio::DestroyAudioManager();
        wm::DestroyInputManager();
        wm::DestroyRenderer();
    }
} // namespace srose
