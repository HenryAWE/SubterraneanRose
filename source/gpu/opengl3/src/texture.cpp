/**
 * @file texture.cpp
 * @author HenryAWE
 * @brief OpenGL texture wrapper
 */

#include <glad/glad.h>
#include <sr/gpu/opengl3/texture.hpp>
#include <cassert>
#include <SDL.h>
#include <stb_image.h>
#include "gl_assert.h"


namespace srose::gpu::opengl3
{
    Texture::~Texture() noexcept
    {
        Destroy();
    }

    void Texture::Generate() noexcept
    {
        SR_ASSERT_CTX();
        glGenTextures(1, &m_handle);
    }
    void Texture::Destroy() noexcept
    {
        SR_ASSERT_CTX();
        if(m_handle)
            glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }

    bool Texture::LoadDefaultTexture()
    {
        SR_ASSERT_CTX();
        if(!m_handle) Generate();
        constexpr int width = 4, height = 4;
        #define SR_BLACK {0, 0, 0, 255}
        #define SR_RED {255, 0, 0, 255}
        constexpr unsigned char data[width][height][4 /*RGBA*/]
        {
            { SR_RED, SR_BLACK, SR_RED, SR_BLACK },
            { SR_BLACK, SR_RED, SR_BLACK, SR_RED },
            { SR_RED, SR_BLACK, SR_RED, SR_BLACK },
            { SR_BLACK, SR_RED, SR_BLACK, SR_RED }
        };

        glBindTexture(GL_TEXTURE_2D, m_handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );
        SR_ASSERT_GL();
        glGenerateMipmap(GL_TEXTURE_2D);
        SR_ASSERT_GL();
        glBindTexture(GL_TEXTURE_2D, 0);
        m_size = {width, height};

        return true;
    }
    bool Texture::LoadFromFile(const char* path)
    {
        Description default_desc;
        default_desc.s = REPEAT;
        default_desc.t = REPEAT;
        default_desc.min = LINEAR;
        default_desc.mag = LINEAR;
        return LoadFromFileEx(path, default_desc);
    }
    bool Texture::LoadFromFileEx(const char* path, const Description& desc)
    {
        SR_ASSERT_CTX();
        if(!m_handle) Generate();

        int width, height;
        stbi_uc* data = stbi_load(path, &width, &height, nullptr, STBI_rgb_alpha);
        if(!data)
        { // Load failed
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[OpenGL3] Load texture \"%s\" failed: %s",
                path, stbi_failure_reason()
            );

            return LoadDefaultTexture();
        }

        glBindTexture(GL_TEXTURE_2D, m_handle);
        auto wrap_s = TranslateDesc(desc.s);
        auto wrap_t = TranslateDesc(desc.t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TranslateDesc(desc.min, true));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TranslateDesc(desc.mag, false));
        if(wrap_s == GL_CLAMP_TO_BORDER || wrap_t == GL_CLAMP_TO_BORDER)
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &desc.border_color[0]);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );
        SR_ASSERT_GL();
        glGenerateMipmap(GL_TEXTURE_2D);
        SR_ASSERT_GL();
        glBindTexture(GL_TEXTURE_2D, 0);

        m_size = {width, height};

        stbi_image_free(data);

        return true;
    }

    bool Texture::LoadFromFile(const filesystem::path& file)
    {
        return LoadFromFile(file.u8string().c_str());
    }
    bool Texture::LoadFromFileEx(const filesystem::path& file, const Description& desc)
    {
        return LoadFromFileEx(file.u8string().c_str(), desc);
    }

    GLenum Texture::TranslateDesc(Filter filter, bool mipmap) noexcept
    {
        switch(filter)
        {
            default:
            case LINEAR: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
            case NEAREST: return mipmap ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR;
        }
    }
    GLenum Texture::TranslateDesc(Wrapping wrap) noexcept
    {
        switch(wrap)
        {
            default:
            case REPEAT: return GL_REPEAT;
            case MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
            case CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
            case CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
        }
    }
} // namespace srose::gpu::opengl3
