/**
 * @file shader.cpp
 * @author HenryAWE
 * @brief Shader program
 */

#include <sr/graphic/opengl3/shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL_log.h>
#include "gl_assert.h"


namespace srose::gpu::opengl3
{
    ShaderProgram::~ShaderProgram() noexcept
    {
        Destroy();
    }

    void ShaderProgram::Generate()
    {
        SR_ASSERT_CTX();
        Destroy();
        m_handle = glCreateProgram();
    }

    void ShaderProgram::Destroy() noexcept
    {
        SR_ASSERT_CTX();
        if(!m_handle)
            return;
        glDeleteProgram(m_handle);
        m_handle = 0;
    }

    bool ShaderProgram::Compile(const char* vssrc, const char* fssrc)
    {
        SR_ASSERT_CTX();

        GLuint vert = 0, frag = 0;
        int status = 0;
        char info[1024] = {};

        vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vssrc, nullptr);
        glCompileShader(vert);
        glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
        if(!status)
        {
            glGetShaderInfoLog(vert, 1024, nullptr, info);
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[OpenGL3] Compile vertex shader error:\n%s",
                info
            );
        }

        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fssrc, nullptr);
        glCompileShader(frag);
        glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
        if(!status)
        {
            glGetShaderInfoLog(frag, 1024, nullptr, info);
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[OpenGL3] Compile fragment shader error:\n%s",
                info
            );
        }

        if(!m_handle)
            Generate();
        glAttachShader(m_handle, vert);
        glAttachShader(m_handle, frag);
        glLinkProgram(m_handle);
        glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
        if(!status)
        {
            glGetProgramInfoLog(m_handle, 1024, nullptr, info);
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[OpenGL3] Link shader program error:\n%s",
                info
            );
        }

        glDeleteShader(vert);
        glDeleteShader(frag);

        return status != 0;
    }

    ShaderProgram::loc_type ShaderProgram::UniformLocation(const char* name)
    {
        return glGetUniformLocation(m_handle, name);
    }

    void Uniform(GLint loc, float v0)
    {
        glUniform1f(loc, v0);
        SR_ASSERT_GL();
    }
    void Uniform(GLint loc, const float (&v)[4])
    {
        glUniform4fv(loc, 1, v);
        SR_ASSERT_GL();
    }
    void Uniform(GLint loc, const glm::vec4& v)
    {
        glUniform4fv(loc, 1, glm::value_ptr(v));
        SR_ASSERT_GL();
    }
    void Uniform(GLint loc, const glm::mat4& m)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
        SR_ASSERT_GL();
    }
} // namespace srose::gpu::opengl3
