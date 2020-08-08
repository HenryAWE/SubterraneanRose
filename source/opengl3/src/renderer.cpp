/**
 * @file renderer.cpp
 * @author HenryAWE
 * @brief OpenGL3 renderer
 */

#include <sr/graphic/opengl3/renderer.hpp>
#include <glad/glad.h>
#include <SDL.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <sr/ui/uimgr.hpp>
#include <sr/trace/log.hpp>
#include "gl_assert.h"
#include "demo.hpp"
#include "render_system.hpp"


const char sprite_vs[] =
R"(#version 330 core
layout(location=0) in vec2 in_vertex;
layout(location=1) in vec2 in_texcoord;
layout(location=2) in mat4 in_tr;
out vec2 texcoord;
uniform mat4 proj;
void main(){
    gl_Position = proj*in_tr*vec4(in_vertex, 0.0f, 1.0f);
    texcoord = in_texcoord;
})";
const char sprite_fs[] =
R"(#version 330 core
in vec2 texcoord;
out vec4 color;
uniform sampler2D tex;
void main(){
    color = texture(tex, texcoord);
})";

namespace srose::graphic::opengl3
{
    Renderer::Renderer(wm::Window& window)
        : graphic::Renderer(window)
    {
        SR_ASSERT_CTX();

        m_default_texture.LoadDefaultTexture();
        InitSpriteRenderer();

        BOOST_LOG_TRIVIAL(info)
            << "[OpenGL3] Renderer Information:\n"
            << Information().c_str();
    }

    void Renderer::ClearScreen(const glm::vec4& color) noexcept
    {
        SR_ASSERT_CTX();
        glClearColor(color[0], color[1], color[2], color[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::Present()
    {
        SR_ASSERT_CTX();
        SDL_GL_SwapWindow(GetWindow().handle());
    }
    void Renderer::Render()
    {
#ifndef SROSE_DISABLE_DEMO
        if(m_demo_initialized)
        {
            m_demo_window->Render();;
        }
#endif
    }

    std::string Renderer::Information()
    {
        SR_ASSERT_CTX();

        using namespace std::literals;
        std::string info;
        info += "Vendor: "s + (const char*)glGetString(GL_VENDOR) + '\n';
        info += "Renderer: "s + (const char*)glGetString(GL_RENDERER) + '\n';
        info += "Version: "s + (const char*)glGetString(GL_VERSION) + '\n';

        return std::move(info);
    }

    void Renderer::ClearScreen()
    {
        ClearScreen(GetCurrentColor());
    }

    void Renderer::ShowDemoWindow(bool* p_open)
    {
        auto& uimgr = ui::UIManager::GetInstance();
#ifndef SROSE_DISABLE_DEMO
        if(!m_demo_initialized)
        {
            m_demo_window = std::make_shared<OpenGL3DemoWindow>(*this, true);
            m_demo_initialized = true;
        }
        m_demo_window->Open();
        m_demo_window->Update();
        if(p_open)
            *p_open = m_demo_window->visible();
#else
        if(p_open)
            *p_open = false;
#endif
    }

    void Renderer::ReleaseUIData() noexcept
    {
#ifndef SROSE_DISABLE_DEMO
        if(!m_demo_initialized)
            return;
        m_demo_window.reset();
        m_demo_initialized = false;
#endif
    }

    void Renderer::AddSprite(const Sprite& sp)
    {
        AppendSpriteData(
            static_cast<Texture*>(sp.GetTexture()),
            sp.CalcMatrix()
        );
    }
    void Renderer::RenderSprite(glm::vec2 viewport, bool clear)
    {
        glUseProgram(m_sprite_shader);
        Uniform(m_sprite_shader.UniformLocation("proj"), glm::ortho(0.0f, viewport.x, 0.0f, viewport.y));
        for(auto& [tex, data] : m_sprite_data)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_sprite_vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(SpriteRenderData), data.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, tex);
            glBindVertexArray(m_sprite_vao);
            glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)data.size());
            glBindVertexArray(0);
            if(clear)
                data.clear();
        }
    }
    void Renderer::ClearSprite()
    {
        for(auto& i : m_sprite_data)
            i.second.clear();
    }

    void Renderer::AddRenderData(player::Stage& stage)
    {
        stage.world.AddGlobalComponent<StageRenderData>(
            CreateScreenTexture(stage.GetSize())
        );
        AddRenderSystem(stage.world.GetSystemManager(stage.world.RENDER));
    }
    void Renderer::RenderStage(player::Stage& stage)
    {
        auto& data = *stage.world.GetGlobalComponent<StageRenderData>();
        auto& screen = static_cast<ScreenTexture&>(*data.screen_texture.get());
        glBindFramebuffer(GL_FRAMEBUFFER, screen.framebuffer());
        glViewport(0, 0, stage.GetSize().x, stage.GetSize().y);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        stage.world.Render();
        RenderSprite(stage.GetSize());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::AddRenderSystem(player::system::SystemManager& smgr)
    {
        using Components = std::tuple<
            player::component::Transform,
            player::component::Image
        >;
        smgr.Add<RenderSystem, Components>(this);
    }

    void Renderer::AppendSpriteData(Texture* tex, const glm::mat4& transform)
    {
        SpriteRenderData data{ transform };
        GLuint handle = tex ? tex->handle() : m_default_texture.handle();
        m_sprite_data[handle].emplace_back(std::move(data));
    }

    Texture* Renderer::NewTexture()
    {
        return new Texture;
    }
    ScreenTexture* Renderer::NewScreenTexture(glm::ivec2 size)
    {
        std::unique_ptr<ScreenTexture> guard(new ScreenTexture);
        guard->Generate(size);
        return guard.release();
    }

    void Renderer::InitImGuiRenderer()
    {
        if(!ImGui_ImplOpenGL3_Init())
            throw std::runtime_error("[WM] ImGui_ImplOpenGL3_Init() failed");
    }
    void Renderer::ShutdownImGuiRenderer()
    {
        ImGui_ImplOpenGL3_Shutdown();
    }
    void Renderer::NewImGuiFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
    }
    void Renderer::RenderImGuiFrame()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Renderer::InitSpriteRenderer()
    {
        SR_ASSERT_CTX();
        m_sprite_vao.Generate();
        glBindVertexArray(m_sprite_vao);
        m_sprite_vbo[0].Generate();
        glBindBuffer(GL_ARRAY_BUFFER, m_sprite_vbo[0]);
        constexpr float sprite_vertices[] = {
            1.0f, 1.0f,   1.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, 1.0f,  0.0f, 0.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(sprite_vertices), sprite_vertices, GL_STATIC_DRAW);
        constexpr unsigned int sprite_indices[] = {
            0, 1, 3,
            1, 2, 3
        };
        m_sprite_ebo.Generate();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sprite_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sprite_indices), sprite_indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
        glEnableVertexAttribArray(1);

        // Set attribute location of the transform matrix by four vec4
        SR_ASSERT_GL();
        m_sprite_vbo[1].Generate();
        glBindBuffer(GL_ARRAY_BUFFER, m_sprite_vbo[1]);
        for(int i : {0, 1, 2, 3})
        {
            int index = i + 2;
            constexpr auto vec4size = sizeof(glm::vec4);
            glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void*)(i * vec4size));
            glEnableVertexAttribArray(index);
            glVertexAttribDivisor(index, 1);
        }
        SR_ASSERT_GL();
        glBindVertexArray(0);
        SR_ASSERT_GL();

        m_sprite_shader.Compile(
            sprite_vs,
            sprite_fs
        );
    }
} // namespace srose::graphic::opengl3
