/**
 * @file demo.hpp
 * @author HenryAWE
 * @brief OpenGL3 Demo for testing
 */

#ifndef SROSE_GPU_OPENGL3_demo_hpp_
#define SROSE_GPU_OPENGL3_demo_hpp_

#include <glad/glad.h>
#include <sr/ui/gui/widget.hpp>
#include <sr/gpu/opengl3/buffer.hpp>
#include <sr/gpu/opengl3/shader.hpp>
#include <sr/gpu/opengl3/texture.hpp>


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

        void UpdateMenuBar();
        void UpdateTabBar();

        bool m_triangle_demo = true; // Open by default
        VertexArray m_triangle_vao;
        Buffer m_triangle_vbo;
        glm::vec4 m_triangle_color = glm::vec4(1);
        ShaderProgram m_triangle_shader;
        glm::ivec4 m_triangle_viewport;
        void TriangleDemoTabItem();

        bool m_texture_demo = false;
        std::string m_texture_location_buffer;
        std::string m_texture_location;
        Texture m_texture_texture;
        glm::vec4 m_texture_uvs = { 0.0f, 0.0f, 1.0f, 1.0f };
        void TextureDemoTabItem();
    };
} // namespace srose::gpu::opengl3


#endif
