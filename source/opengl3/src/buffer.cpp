/**
 * @file buffer.cpp
 * @author HenryAWE
 * @brief OpenGL buffer manager
 */

#include <glad/glad.h>
#include <sr/graphic/opengl3/buffer.hpp>
#include "gl_assert.h"


namespace srose::gpu::opengl3
{
    Buffer::~Buffer() noexcept
    {
        Destroy();
    }

    void Buffer::Generate()
    {
        SR_ASSERT_CTX();
        Destroy();
        glGenBuffers(1, &m_handle);
    }

    void Buffer::Destroy() noexcept
    {
        SR_ASSERT_CTX();
        if(!m_handle)
            return;
        glDeleteBuffers(1, &m_handle);
        m_handle = 0;
    }

    VertexArray::~VertexArray() noexcept
    {
        Destroy();
    }

    void VertexArray::Generate()
    {
        SR_ASSERT_CTX();
        Destroy();
        glGenVertexArrays(1, &m_handle);
    }

    void VertexArray::Destroy() noexcept
    {
        SR_ASSERT_CTX();
        if(!m_handle)
            return;
        glDeleteVertexArrays(1, &m_handle);
    }

    Framebuffer::~Framebuffer() noexcept
    {
        Destroy();
    }

    void Framebuffer::Generate()
    {
        SR_ASSERT_CTX();
        Destroy();
        glGenFramebuffers(1, &m_handle);
    }

    void Framebuffer::Destroy() noexcept
    {
        SR_ASSERT_CTX();
        if(!m_handle)
            return;
        glDeleteFramebuffers(1, &m_handle);
    }

    Renderbuffer::~Renderbuffer() noexcept
    {
        Destroy();
    }

    void Renderbuffer::Generate()
    {
        SR_ASSERT_CTX();
        Destroy();
        glGenRenderbuffers(1, &m_handle);
    }

    void Renderbuffer::Destroy() noexcept
    {
        SR_ASSERT_CTX();
        if(!m_handle)
            return;
        glDeleteRenderbuffers(1, &m_handle);
    }
} // namespace srose::gpu::opengl3
