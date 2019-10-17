/**
 * @file shader.hpp
 * @author HenryAWE
 * @brief Shader program
 */

#ifndef SROSE_GPU_OPENGL3_shader_hpp_
#define SROSE_GPU_OPENGL3_shader_hpp_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utility>


namespace srose::gpu::opengl3
{
    class ShaderProgram
    {
    public:
        typedef GLuint handle_type;
        typedef GLint loc_type;

        ShaderProgram() noexcept = default ;
        ShaderProgram(const ShaderProgram& rhs) = delete;
        ShaderProgram(ShaderProgram&& move) noexcept
            : m_handle(std::exchange(move.m_handle, 0)) {}

        ~ShaderProgram() noexcept;

        void Generate();

        void Destroy() noexcept;

        bool Compile(const char* vssrc, const char* fssrc);

        loc_type UniformLocation(const char* name);

        [[nodiscard]]
        constexpr handle_type handle() const noexcept { return m_handle; }
        [[nodiscard]]
        constexpr operator handle_type() const noexcept { return m_handle; }
        
    private:
        handle_type m_handle = 0;
    };

    void Uniform(GLint loc, const float (&v)[4]);
    void Uniform(GLint loc, const glm::vec4& v);
} // namespace srose::gpu::opengl3


#endif
