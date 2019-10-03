/**
 * @file texture.cpp
 * @author your name (you@domain.com)
 * @brief OpenGL texture wrapper
 */

#include <glad/glad.h>
#include <sr/gpu/opengl3/texture.hpp>
#include <cassert>
#include <SDL_surface.h>
#include <SDL_image.h>
#include <SDL_pixels.h>


namespace srose::gpu::opengl3
{
    Texture::~Texture() noexcept
    {
        Destroy();
    }

    void Texture::Generate() noexcept
    {
        glGenTextures(1, &m_handle);
    }
    void Texture::Destroy() noexcept
    {
        if(m_handle)
            glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }

    bool Texture::LoadFromFile(const char* path)
    {
        SDL_Surface* loaded = IMG_Load(path);
        if(!loaded)
        {
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[OpenGL3] Texture::LoadFromFile() load %s failed %s",
                path,
                IMG_GetError()
            );
            return false;
        }

        SDL_Surface* surface = loaded;
        if(surface->format->format != SDL_PIXELFORMAT_RGBA8888)
        {
            surface = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA8888, 0);
            if(!surface)
            {
                SDL_LogError(
                    SDL_LOG_CATEGORY_APPLICATION,
                    "[OpenGL3] Texture::LoadFromFile() convert data from %s to RGBA8888 failed",
                    SDL_GetPixelFormatName(surface->format->format)
                );

                SDL_FreeSurface(loaded);
                return false;
            }
        }

        if(!m_handle) Generate();
        glBindTexture(GL_TEXTURE_2D, m_handle);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            surface->w,
            surface->h,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            surface->pixels
        );
        assert(glGetError() == GL_NO_ERROR);
        glGenerateMipmap(GL_TEXTURE_2D);
        assert(glGetError() == GL_NO_ERROR);
        glBindTexture(GL_TEXTURE_2D, 0);
        assert(glGetError() == GL_NO_ERROR);

        m_size = {surface->w, surface->h};

        if(surface != loaded) SDL_FreeSurface(surface);

        return true;
    }
} // namespace srose::gpu::opengl3
