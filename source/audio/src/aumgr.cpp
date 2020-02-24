/**
 * @file aumgr.cpp
 * @author HenryAWE
 * @brief Audio manager
 */

#include <sr/audio/aumgr.hpp>
#include <memory>
#include <sr/ui/gui/uimgr.hpp>
#include "demo.hpp"


namespace srose::audio
{
    AudioManager::AudioManager()
    {

    }

    AudioManager::~AudioManager() noexcept
    {

    }

    void AudioManager::ShowDemoWindow(bool* p_open)
    {
        auto& uimgr = *ui::GetUIManager();
        if(!m_demo_initialized)
        {
            uimgr.widget_tree.emplace_at(
                "srose.audio.demo",
                std::make_shared<AudioDemoWindow>()
            );

            m_demo_initialized = true;
        }
        auto ptr = uimgr.widget_tree["srose.audio.demo"].get();
        static_cast<AudioDemoWindow*>(ptr)->open = true;
        ptr->Update();
        if(p_open)
            *p_open = static_cast<AudioDemoWindow*>(ptr)->open;
    }

    std::unique_ptr<AudioManager> g_aumgr;

    AudioManager* SRSCALL CreateAudioManager()
    {
        g_aumgr = std::make_unique<AudioManager>();
        return g_aumgr.get();
    }
    void SRSCALL DestroyAudioManager() noexcept
    {
        g_aumgr.reset();
    }
    [[nodiscard]]
    AudioManager* SRSCALL GetAudioManager() noexcept
    {
        return g_aumgr.get();
    }
} // namespace srose::audio
