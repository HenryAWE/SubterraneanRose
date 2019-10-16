/**
 * @file shader.cpp
 * @author HenryAWE
 * @brief Shader program
 */

#include <sr/gpu/opengl3/shader.hpp>


namespace srose::gpu::opengl3
{
    void ShaderProgram::Generate()
    {
        Destroy();
        m_handle = glCreateProgram();
    }

    void ShaderProgram::Destroy() noexcept
    {
        if(!m_handle)
            return;
        glDeleteProgram(m_handle);
        m_handle = 0;
    }
} // namespace srose::gpu::opengl3
