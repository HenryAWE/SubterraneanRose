/**
 * @file aumgr.hpp
 * @author HenryAWE
 * @brief Audio manager
 */

#ifndef SROSE_AUDIO_aumgr_hpp_
#define SROSE_AUDIO_aumgr_hpp_

#include <memory>
#include <sr/core/macros.hpp>
#include "audio.hpp"


namespace srose::audio
{
#ifndef SROSE_DISABLE_DEMO
    class AudioDemoWindow;
#endif

    class AudioManager
    {
    public:
        AudioManager();
        AudioManager(const AudioManager&) = delete;

        ~AudioManager() noexcept;

        void ShowDemoWindow(bool* p_open = nullptr);

    private:
#ifndef SROSE_DISABLE_DEMO
        std::shared_ptr<AudioDemoWindow> m_demo_window;
        bool m_demo_initialized = false;
#endif
    };

    AudioManager* SRSCALL CreateAudioManager();
    void SRSCALL DestroyAudioManager() noexcept;
    [[nodiscard]]
    AudioManager* SRSCALL GetAudioManager() noexcept;
} // namespace srose::audio


#endif
