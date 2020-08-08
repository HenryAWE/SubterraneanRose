/**
 * @file audio.cpp
 * @author HenryAWE
 * @brief Wrapper for SDL_mixer
 */

#include <sr/audio/audio.hpp>
#include <sr/trace/log.hpp>


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
            BOOST_LOG_TRIVIAL(error)
                << "[Audio] Mix_LoadMUS(\""
                << file.u8string()
                << "\") failed";
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
            BOOST_LOG_TRIVIAL(error)
                << "[Audio] Mix_LoadWAV(\""
                + file.u8string()
                + "\") failed: ";
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
