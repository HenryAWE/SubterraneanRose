/**
 * @file renderer.hpp
 * @author HenryAWE
 * @brief OpenGL3 renderer
 */

#ifndef SROSE_GPU_OPENGL3_renderer_hpp_
#define SROSE_GPU_OPENGL3_renderer_hpp_

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <sr/wm/display.h>
#include <sr/gpu/renderer.hpp>
#include "texture.hpp"
#include "buffer.hpp"
#include "shader.hpp"


namespace srose::gpu::opengl3
{
    class Renderer : public gpu::Renderer
    {
        SR_WM_display* m_disp = nullptr;

    public:
        Renderer(SR_WM_display* disp);

        void ClearScreen(const glm::vec4& color) noexcept;

        void Present() noexcept;

        std::string Information();

        void ClearScreen() override;
    };
} // namespace srose::gpu::opengl3


#endif
