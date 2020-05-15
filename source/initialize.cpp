/**
 * @file initialize.cpp
 * @author HenryAWE
 * @brief Initializer
 */

#include "initialize.hpp"
#include <stdexcept>
#include <future>
#include <SDL.h>
#include <imgui.h>
#include <sr/audio/aumgr.hpp>
#include <sr/res/resmgr.hpp>
#include <sr/wm/input.hpp>
#include <sr/ui/uimgr.hpp>
#include <sr/player/player.hpp>
#include <sr/srose/app.hpp>
#include <sr/wm/event.hpp>
#include <sr/gpu/renderer.hpp>
#ifdef __WINDOWS__
#   include <comdef.h>
#   include <combaseapi.h>
#endif


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

    void InitializeAllSystems(wm::Window& window)
    {
    #ifdef __WINDOWS__
        if(::HRESULT hr = ::CoInitialize(nullptr); FAILED(hr))
            _com_raise_error(hr);
    #endif
        wm::InitEventSystem(window);
        GetApp().LoadUsers();
        auto font_ready = std::async(std::launch::async, LoadFonts);
        audio::CreateAudioManager();
        res::CreateResourceManager();
        wm::CreateInputManager();
        font_ready.get();
        ui::UIManager::GetInstance().Initialize(window);
    }
    void DeinitializeAllSystems(wm::Window& window) noexcept
    {
        player::ReleaseUIData();
        window.GetRenderer().ReleaseUIData();
        ui::UIManager::GetInstance().Deinitialize();
        res::DestroyResourceManager();
        audio::DestroyAudioManager();
        wm::DestroyInputManager();
    #ifdef __WINDOWS__
        ::CoUninitialize();
    #endif
    }
} // namespace srose
