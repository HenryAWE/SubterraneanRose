/**
 * @file renderer.hpp
 * @author HenryAWE
 * @brief OpenGL3 renderer
 */

#ifndef SROSE_GPU_OPENGL3_renderer_hpp_
#define SROSE_GPU_OPENGL3_renderer_hpp_

#include <glm/glm.hpp>


namespace srose::gpu::opengl3
{
    class renderer
    {
    public:
        void ClearScreen(glm::vec4 color) noexcept;
    };
} // namespace srose::gpu::opengl3


#endif
