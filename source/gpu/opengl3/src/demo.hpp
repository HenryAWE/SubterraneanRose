/**
 * @file demo.hpp
 * @author HenryAWE
 * @brief OpenGL3 Demo for testing
 */

#ifndef SROSE_GPU_OPENGL3_demo_hpp_
#define SROSE_GPU_OPENGL3_demo_hpp_

#include <glad/glad.h>
#include <sr/ui/gui/widget.hpp>


namespace srose::gpu::opengl3
{
    class OpenGL3DemoWindow : public ui::Widget
    {
    public:
        OpenGL3DemoWindow(bool initgl = true);

        ~OpenGL3DemoWindow();

        void InitializeGL();
        void ReleaseGL() noexcept;

        [[nodiscard]]
        bool gl_initialized() const noexcept { return m_gl_initialized; }

        bool open = true;
        void Update() override;

    private:
        bool m_gl_initialized = false;
    };
} // namespace srose::gpu::opengl3


#endif
