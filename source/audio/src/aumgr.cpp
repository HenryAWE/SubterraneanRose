/**
 * @file aumgr.cpp
 * @author HenryAWE
 * @brief Audio manager
 */

#include <sr/audio/aumgr.hpp>
#include <memory>


namespace srose::audio
{
    AudioManager::AudioManager()
    {

    }

    AudioManager::~AudioManager() noexcept
    {

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
