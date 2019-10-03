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
        int width, height;
        stbi_uc* data = stbi_load(path, &width, &height, nullptr, STBI_rgb_alpha);

        if(!m_handle) Generate();
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
        assert(glGetError() == GL_NO_ERROR);
        glGenerateMipmap(GL_TEXTURE_2D);
        assert(glGetError() == GL_NO_ERROR);
        glBindTexture(GL_TEXTURE_2D, 0);
        assert(glGetError() == GL_NO_ERROR);

        m_size = {width, height};

        stbi_image_free(data);

        return true;
    }
} // namespace srose::gpu::opengl3
