/**
 * @file audio.cpp
 * @author HenryAWE
 * @brief Wrapper for SDL_mixer
 */

#include <sr/audio/audio.hpp>
#include <SDL_log.h>


namespace srose::audio
{
    Music::~Music() noexcept
    {
        Destroy();
    }

    bool Music::Load(const filesystem::path& file)
    {
        auto tmp = Mix_LoadMUS(file.u8string().c_str());
        if(!tmp)
        {
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[Audio] Mix_LoadMUS() load file \"%s\" failed: %s",
                file.u8string().c_str(),
                Mix_GetError()
            );
            return false;
        }
        if(m_handle) Mix_FreeMusic(m_handle);
        m_handle = tmp;

        return true;
    }
    void Music::Destroy() noexcept
    {
        if(m_handle)
        {
            Mix_FreeMusic(m_handle);
            m_handle = nullptr;
        }
    }

    Audio::~Audio() noexcept
    {
        Destroy();
    }

    bool Audio::Load(const filesystem::path& file)
    {
        auto tmp = Mix_LoadWAV(file.u8string().c_str());
        if(!tmp)
        {
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[Audio] Mix_LoadWAV() load file \"%s\" failed: %s",
                file.u8string().c_str(),
                Mix_GetError()
            );
            return false;
        }
        if(m_handle) Mix_FreeChunk(m_handle);
        m_handle = tmp;

        return true;
    }
    void Audio::Destroy() noexcept
    {
        if(m_handle)
        {
            Mix_FreeChunk(m_handle);
            m_handle = nullptr;
        }
    }
} // namespace srose::audio
