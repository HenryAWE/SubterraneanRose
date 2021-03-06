/**
 * @file demo.hpp
 * @author HenryAWE
 * @brief OpenGL3 Demo for testing
 */

#ifndef SROSE_GRAPHIC_OPENGL3_demo_hpp_
#define SROSE_GRAPHIC_OPENGL3_demo_hpp_

#include <glad/glad.h>
#include <vector>
#include <sr/ui/node.hpp>
#include <sr/graphic/opengl3/renderer.hpp>
#include <sr/graphic/opengl3/buffer.hpp>
#include <sr/graphic/opengl3/shader.hpp>
#include <sr/graphic/opengl3/texture.hpp>
#include <sr/graphic/opengl3/effect.hpp>


#ifndef SROSE_DISABLE_DEMO

namespace srose::graphic::opengl3
{
    class OpenGL3DemoWindow : public ui::StandaloneNode
    {
        Renderer* m_ren;
    public:
        typedef StandaloneNode Base;

        OpenGL3DemoWindow(Renderer& ren, bool initgl = true);

        ~OpenGL3DemoWindow();

        void InitializeGL();
        void ReleaseGL() noexcept;

        [[nodiscard]]
        bool gl_initialized() const noexcept { return m_gl_initialized; }

        void Render();

        void Update() override;

    private:
        bool m_gl_initialized = false;

        void UpdateMenuBar();
        void UpdateTabBar();

        bool m_integrate_demo = true; // Open by default
        ScreenTexture m_integrate_frametex;
        void IntegrateDemoTabItem();

        bool m_triangle_demo = false;
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
        Texture::Description m_texture_desc = {};
        bool m_texture_descchanged = false;
        void TextureDemoTabItem();

        bool m_effect_demo = false;
        VertexArray m_effect_vao;
        Buffer m_effect_vbo;
        Buffer m_effect_ebo;
        std::vector<ShaderProgram> m_effect_shaders;
        std::size_t m_effect_id;
        glm::ivec4 m_effect_viewport;
        void EffectDemoTabItem();
    };
} // namespace srose::graphic::opengl3

#endif


#endif
