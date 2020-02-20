/**
 * @file audio.hpp
 * @author HenryAWE
 * @brief Wrapper for SDL_mixer
 */

#ifndef SROSE_AUDIO_audio_hpp_
#define SROSE_AUDIO_audio_hpp_

#include <SDL_mixer.h>
#include <sr/filesystem/filesystem.hpp>


namespace srose::audio
{
    class Music
    {
        Mix_Music* m_handle = nullptr;
    public:
        typedef Mix_Music* handle_type;

        ~Music() noexcept;

        bool Load(const filesystem::path& file);
        void Destroy() noexcept;

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }
    };
    class Audio
    {
        Mix_Chunk* m_handle = nullptr;
    public:
        typedef Mix_Chunk* handle_type;

        ~Audio() noexcept;

        bool Load(const filesystem::path& file);
        void Destroy() noexcept;

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }
    };
} // namespace srose::audio


#endif
