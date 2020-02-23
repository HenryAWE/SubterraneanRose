/**
 * @file aumgr.hpp
 * @author HenryAWE
 * @brief Audio manager
 */

#ifndef SROSE_AUDIO_aumgr_hpp_
#define SROSE_AUDIO_aumgr_hpp_

#include <sr/core/macros.h>
#include "audio.hpp"


namespace srose::audio
{
    class AudioManager
    {
    public:
        AudioManager();
        AudioManager(const AudioManager&) = delete;

        ~AudioManager() noexcept;
    };

    AudioManager* SRSCALL CreateAudioManager();
    void SRSCALL DestroyAudioManager() noexcept;
    [[nodiscard]]
    AudioManager* SRSCALL GetAudioManager() noexcept;
} // namespace srose::audio


#endif
