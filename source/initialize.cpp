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
#include <sr/core/init.hpp>
#include <sr/audio/aumgr.hpp>
#include <sr/res/resmgr.hpp>
#include <sr/wm/input.hpp>
#include <sr/ui/uimgr.hpp>
#include <sr/player/player.hpp>
#include <sr/srose/app.hpp>
#include <sr/wm/event.hpp>
#include <sr/graphic/renderer.hpp>
#ifdef _WIN32
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

    void InitializeAllSystems(wm::Window& window, bool disable_audio)
    {
    #ifdef _WIN32
        if(::HRESULT hr = ::CoInitialize(nullptr); FAILED(hr))
            _com_raise_error(hr);
    #endif
        GetApp().LoadUsers();
        auto font_ready = std::async(std::launch::async, LoadFonts);

        if(!disable_audio)
        {
            core::InitSDLMixer();
            audio::CreateAudioManager();
        }
        res::CreateResourceManager();
        wm::CreateInputManager();
        font_ready.get();
        ui::UIManager::GetInstance().Initialize(window);
    }
    void DeinitializeAllSystems(wm::Window& window, bool disable_audio) noexcept
    {
        player::ReleaseUIData();
        window.GetRenderer().ReleaseUIData();
        ui::UIManager::GetInstance().Deinitialize();
        res::DestroyResourceManager();
        if(!disable_audio)
        {
            audio::DestroyAudioManager();
            core::QuitSDLMixer();
        }
        wm::DestroyInputManager();
    #ifdef _WIN32
        ::CoUninitialize();
    #endif
    }
} // namespace srose
