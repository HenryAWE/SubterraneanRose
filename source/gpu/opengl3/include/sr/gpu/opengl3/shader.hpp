/**
 * @file shader.hpp
 * @author HenryAWE
 * @brief Shader program
 */

#ifndef SROSE_GPU_OPENGL3_shader_hpp_
#define SROSE_GPU_OPENGL3_shader_hpp_

#include <glad/glad.h>
#include <utility>


namespace srose::gpu::opengl3
{
    class ShaderProgram
    {
    public:
        typedef GLuint handle_type;

        ShaderProgram() noexcept = default ;
        ShaderProgram(const ShaderProgram& rhs) = delete;
        ShaderProgram(ShaderProgram&& move) noexcept
            : m_handle(std::exchange(move.m_handle, 0)) {}

        ~ShaderProgram() noexcept;

        void Generate();

        void Destroy() noexcept;

        bool Compile(const char* vssrc, const char* fssrc);

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }
        
    private:
        handle_type m_handle = 0;
    };
} // namespace srose::gpu::opengl3


#endif
