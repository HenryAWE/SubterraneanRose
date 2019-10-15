/**
 * @file buffer.cpp
 * @author HenryAWE
 * @brief OpenGL buffer manager
 */

#include <glad/glad.h>
#include <sr/gpu/opengl3/buffer.hpp>


namespace srose::gpu::opengl3
{
    Buffer::~Buffer() noexcept
    {
        Destroy();
    }

    void Buffer::Generate()
    {
        Destroy();
        glGenBuffers(1, &m_handle);
    }

    void Buffer::Destroy() noexcept
    {
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
        Destroy();
        glGenVertexArrays(1, &m_handle);
    }

    void VertexArray::Destroy() noexcept
    {
        if(!m_handle)
            return;
        glDeleteVertexArrays(1, &m_handle);
    }
} // namespace srose::gpu::opengl3
