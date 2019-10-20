/**
 * @file texture.cpp
 * @author your name (you@domain.com)
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

    bool Texture::LoadFromFile(const char* path)
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

            return false;
        }

        glBindTexture(GL_TEXTURE_2D, m_handle);
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
} // namespace srose::gpu::opengl3
