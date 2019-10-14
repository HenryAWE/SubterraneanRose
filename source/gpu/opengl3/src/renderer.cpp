/**
 * @file renderer.cpp
 * @author HenryAWE
 * @brief OpenGL3 renderer
 */

#include <sr/gpu/opengl3/renderer.hpp>
#include <glad/glad.h>


namespace srose::gpu::opengl3
{
    void renderer::ClearScreen(glm::vec4 color) noexcept
    {
        glClearColor(color[0], color[1], color[2], color[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }
} // namespace srose::gpu::opengl3
