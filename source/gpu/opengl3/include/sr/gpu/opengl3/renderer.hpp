/**
 * @file renderer.hpp
 * @author HenryAWE
 * @brief OpenGL3 renderer
 */

#ifndef SROSE_GPU_OPENGL3_renderer_hpp_
#define SROSE_GPU_OPENGL3_renderer_hpp_

#include <glm/glm.hpp>
#include <sr/wm/display.h>


namespace srose::gpu::opengl3
{
    class Renderer
    {
        SR_WM_display* m_disp = nullptr;

    public:
        Renderer(SR_WM_display* disp);

        void ClearScreen(glm::vec4 color) noexcept;

        void Present() noexcept;
    };
} // namespace srose::gpu::opengl3


#endif
