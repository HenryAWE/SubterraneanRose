/**
 * @file aumgr.cpp
 * @author HenryAWE
 * @brief Audio manager
 */

#include <sr/audio/aumgr.hpp>
#include <memory>
#include <sr/ui/uimgr.hpp>
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
        auto& uimgr = ui::UIManager::GetInstance();
#ifndef SROSE_DISABLE_DEMO
        if(!m_demo_initialized)
        {
            m_demo_window = std::make_shared<AudioDemoWindow>();
            m_demo_initialized = true;
        }
        m_demo_window->Open();
        m_demo_window->Update();
        if(p_open)
            *p_open = m_demo_window->visible();
#else
        if(p_open)
            *p_open = false;
#endif
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
