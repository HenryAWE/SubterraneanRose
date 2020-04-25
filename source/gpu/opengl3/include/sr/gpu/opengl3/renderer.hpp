/**
 * @file renderer.hpp
 * @author HenryAWE
 * @brief OpenGL3 renderer
 */

#ifndef SROSE_GPU_OPENGL3_renderer_hpp_
#define SROSE_GPU_OPENGL3_renderer_hpp_

#include <glad/glad.h>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <sr/wm/display.hpp>
#include <sr/gpu/renderer.hpp>
#include "texture.hpp"
#include "buffer.hpp"
#include "shader.hpp"


namespace srose::gpu::opengl3
{
    class Renderer : public gpu::Renderer
    {
    public:
        Renderer(Display* disp);

        void ClearScreen(const glm::vec4& color) noexcept;

        void Present() override;

        std::string Information();

        void ClearScreen() override;
        void Render() override;

        std::unique_ptr<Texture> CreateTexture() { return std::unique_ptr<Texture>(NewTexture()); }
        std::unique_ptr<ScreenTexture> CreateScreenTexture(glm::ivec2 size) { return std::unique_ptr<ScreenTexture>(NewScreenTexture(size)); }

        void ShowDemoWindow(bool* p_open = nullptr) override;

        void ReleaseUIData() noexcept override;

        void AddSprite(const Sprite& sp) override;
        void RenderSprite(glm::vec2 viewport, bool clear = true) override;
        void ClearSprite() override;

        void AddRenderData(player::Stage& stage) override;
        void RenderStage(player::Stage& stage) override;

        void AddRenderSystem(player::system::SystemManager& smgr) override;

    protected:
        void AppendSpriteData(Texture* tex, const glm::mat4& transform);
#ifndef SROSE_DISABLE_DEMO
        bool m_demo_initialized = false;
#endif

        Texture m_default_texture;

        Texture* NewTexture() override;
        ScreenTexture* NewScreenTexture(glm::ivec2 size) override;

        VertexArray m_sprite_vao;
        Buffer m_sprite_vbo[2];
        Buffer m_sprite_ebo;
        ShaderProgram m_sprite_shader;
        struct SpriteRenderData
        {
            glm::mat4 transform;
        };
        std::map<GLuint /* Texture ID */, std::vector<SpriteRenderData>> m_sprite_data;
        void InitSpriteRenderer();
    };
} // namespace srose::gpu::opengl3


#endif
