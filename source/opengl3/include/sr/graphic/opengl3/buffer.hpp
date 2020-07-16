/**
 * @file buffer.hpp
 * @author HenryAWE
 * @brief OpenGL buffer and vertex array manager
 */

#ifndef SROSE_GPU_OPENGL3_buffer_hpp_
#define SROSE_GPU_OPENGL3_buffer_hpp_

#include <glad/glad.h>
#include <utility>


namespace srose::gpu::opengl3
{
    class Buffer
    {
    public:
        typedef GLuint handle_type;

        Buffer() noexcept = default;
        Buffer(const Buffer& rhs) = delete;
        Buffer(Buffer&& move) noexcept
            : m_handle(std::exchange(move.m_handle, 0)) {}

        ~Buffer() noexcept;

        void Generate();

        void Destroy() noexcept;

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }

    private:
        handle_type m_handle = 0;
    };

    class VertexArray
    {
    public:
        typedef GLuint handle_type;

        VertexArray() noexcept = default;
        VertexArray(const VertexArray& rhs) = delete;
        VertexArray(VertexArray&& move) noexcept
            : m_handle(std::exchange(move.m_handle, 0)) {}

        ~VertexArray() noexcept;
        
        void Generate();

        void Destroy() noexcept;

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }

    private:
        handle_type m_handle = 0;
    };

    class Framebuffer
    {
    public:
        typedef GLuint handle_type;

        Framebuffer() noexcept = default;
        Framebuffer(const Framebuffer& rhs) = delete;
        Framebuffer(Framebuffer&& move) noexcept
            : m_handle(std::exchange(move.m_handle, 0)) {}

        ~Framebuffer() noexcept;
        
        void Generate();

        void Destroy() noexcept;

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }

    private:
        handle_type m_handle = 0;
    };

    class Renderbuffer
    {
    public:
        typedef GLuint handle_type;

        Renderbuffer() noexcept = default;
        Renderbuffer(const Renderbuffer& rhs) = delete;
        Renderbuffer(Renderbuffer&& move) noexcept
            : m_handle(std::exchange(move.m_handle, 0)) {}

        ~Renderbuffer() noexcept;
        
        void Generate();

        void Destroy() noexcept;

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }

    private:
        handle_type m_handle = 0;
    };
} // namespace srose::gpu::opengl3


#endif
