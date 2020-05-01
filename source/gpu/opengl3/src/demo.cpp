/**
 * @file demo.cpp
 * @author HenryAWE
 * @brief OpenGL3 Demo for testing
 */

#include <glad/glad.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "demo.hpp"
#include "gl_assert.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imguisr.h>
#include <sr/gpu/opengl3/renderer.hpp>
#include <sr/gpu/sprite.hpp>


#ifndef SROSE_DISABLE_DEMO

namespace srose::gpu::opengl3
{
    namespace detail
    {
        bool SetupScissorsAndViewport(
            ImDrawData* draw_data,
            const ImVec4& clip,
            const glm::ivec4& view
        ) noexcept {
            ImVec2 clip_off = draw_data->DisplayPos;
            ImVec2 clip_scale = draw_data->FramebufferScale;
            ImVec4 clip_rect;
            int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
            int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
            if (fb_width <= 0 || fb_height <= 0)
                return false;
            clip_rect.x = (clip.x - clip_off.x) * clip_scale.x;
            clip_rect.y = (clip.y - clip_off.y) * clip_scale.y;
            clip_rect.z = (clip.z - clip_off.x) * clip_scale.x;
            clip_rect.w = (clip.w - clip_off.y) * clip_scale.y;
            if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
            {
                glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
                glViewport(view.x, fb_height - view.y, view.z, view.w);
                return true;
            }
            else
            {
                return false;
            }
        }
    } // namespace detail

    OpenGL3DemoWindow::OpenGL3DemoWindow(Renderer& ren, bool initgl)
        : m_ren(&ren)
    {
        if(initgl)
            InitializeGL();
    }
    OpenGL3DemoWindow::~OpenGL3DemoWindow()
    {
        ReleaseGL();
    }

    void OpenGL3DemoWindow::InitializeGL()
    {
        /* Integrate Demo */
        m_integrate_frametex.Generate({960, 720});

        /* Triangle demo */
        m_triangle_vao.Generate();
        m_triangle_vbo.Generate();
        glBindVertexArray(m_triangle_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_triangle_vbo);
        constexpr float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_triangle_shader.Generate();
        m_triangle_shader.Compile(
            "#version 330 core\n"
            "layout(location=0) in vec3 inpos;"
            "void main()"
            "{"
            "    gl_Position=vec4(inpos, 1.0f);"
            "}",
            "#version 330 core\n"
            "out vec4 outcolor;"
            "uniform vec4 unicolor;"
            "void main()"
            "{"
            "    outcolor = unicolor;"
            "}"
        );

        /* Texture Demo */
        m_texture_location_buffer.resize(256);
        m_texture_location.reserve(256);
        m_texture_texture.LoadDefaultTexture();

        /* Effect Demo */
        m_effect_vao.Generate();
        m_effect_vbo.Generate();
        m_effect_ebo.Generate();
        glBindVertexArray(m_effect_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_effect_vbo);
        constexpr float rect_vertices[] = {
            1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 0.0f,  0.0f, 0.0f
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
        constexpr unsigned int rect_indices[] = {
            0, 1, 3,
            1, 2, 3
        };
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_effect_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        m_effect_id = 0;
        m_effect_shaders.reserve(static_cast<int>(EffectID::EffectID_MAX) + 1);
        m_effect_shaders.emplace_back().Compile(
            "#version 330 core\n"
            "layout(location=0) in vec3 inpos;"
            "layout(location=1) in vec2 intexcoord;"
            "out vec2 texcoord;"
            "void main()"
            "{"
            "    gl_Position=vec4(inpos, 1.0f);"
            "    texcoord = intexcoord;"
            "}",
            "#version 330 core\n"
            "in vec2 texcoord;"
            "out vec4 outcolor;"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "    outcolor = texture(tex, texcoord);"
            "}"
        );
        m_effect_shaders.emplace_back(CreateEffect(EffectID::INVERSION));
        m_effect_shaders.emplace_back(CreateEffect(EffectID::GRAYSCALE));
        m_effect_shaders.emplace_back(CreateEffect(EffectID::SHARPEN));
        m_effect_shaders.emplace_back(CreateEffect(EffectID::BLUR));

        m_gl_initialized = true;
    }
    void OpenGL3DemoWindow::ReleaseGL() noexcept
    {
        m_gl_initialized = false;
        m_triangle_vao.Destroy();
        m_triangle_vbo.Destroy();
        m_triangle_shader.Destroy();

        m_texture_texture.Destroy();

        m_effect_vao.Destroy();
        m_effect_vbo.Destroy();
        m_effect_ebo.Destroy();
        for(auto& i : m_effect_shaders)
            i.Destroy();
        m_effect_id = 0;
    }

    void OpenGL3DemoWindow::Render()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_integrate_frametex.framebuffer());
        auto view = m_integrate_frametex.size();
        glViewport(0, 0, view.x, view.y);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        static float t = 0.0f;
        for(int i = 0; i < 80; ++i)
        {
            Sprite sp;
            sp.SetTexture(&m_texture_texture);
            sp.position({i * 10.0f, std::pow(i - 40, 2) * 0.5f + 10.0f});
            sp.scale({10, 10});
            sp.rotation(t += ImGui::GetIO().DeltaTime);
            m_ren->AddSprite(sp);
        }

        Sprite sp2;
        sp2.SetTexture(nullptr); // Should use the default texture
        sp2.position(glm::vec2(960/2, 720/2) + 50.0f*glm::vec2(cos(t/50.0f), sin(t/50.0f)));
        sp2.scale({25, 25});
        m_ren->AddSprite(sp2);

        m_ren->RenderSprite({960, 720});
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGL3DemoWindow::Update()
    {
        assert(m_gl_initialized);
        auto& io = ImGui::GetIO();

        constexpr int background_flags = 
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_MenuBar;
        ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_Appearing);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            "OpenGL3 Demo",
            &open,
            background_flags
        );
        if(!background)
            return;

        UpdateMenuBar();

        bool any_tab =
            m_integrate_demo ||
            m_triangle_demo ||
            m_texture_demo ||
            m_effect_demo;
        if(!any_tab)
        {
            ImGui::TextDisabled("(Empty)");
            return;
        }
        else
        {
            UpdateTabBar();
        }
    }

    void OpenGL3DemoWindow::UpdateMenuBar()
    {
        auto menubar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_MenuBar>();
        if(menubar)
        {
            auto demo_list = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Menu>("Demo List");
            if(demo_list)
            {
                ImGui::Checkbox("Integrate Demo", &m_integrate_demo);
                ImGui::Checkbox("Triangle Demo", &m_triangle_demo);
                ImGui::Checkbox("Texture Demo", &m_texture_demo);
                ImGui::Checkbox("Effect Demo", &m_effect_demo);
            }
        }
    }
    void OpenGL3DemoWindow::UpdateTabBar()
    {
        constexpr int tabbar_flags = 
            ImGuiTabBarFlags_NoTooltip |
            ImGuiTabBarFlags_AutoSelectNewTabs |
            ImGuiTabBarFlags_Reorderable;
        auto tabbar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabBar>("##tabbar", tabbar_flags);
        if(tabbar)
        {
            if(m_integrate_demo) IntegrateDemoTabItem();
            if(m_triangle_demo) TriangleDemoTabItem();
            if(m_texture_demo) TextureDemoTabItem();
            if(m_effect_demo) EffectDemoTabItem();
        }
    }

    void OpenGL3DemoWindow::IntegrateDemoTabItem()
    {
        auto tabitem =  ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabItem>(
            "Integrate Demo",
            &m_integrate_demo
        );
        if(!tabitem)
            return;
        
        auto framesize = m_integrate_frametex.size();
        ImGui::Text("Frame size: %dx%d", framesize.x, framesize.y);
        if(ImGui::BeginChild("##frametex"))
        {
            ImGui::Image(
                m_integrate_frametex.GetNativeHandle(),
                ImGui::GetWindowSize(),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }
        ImGui::EndChild();
    }
    void OpenGL3DemoWindow::TriangleDemoTabItem()
    {
        auto& io = ImGui::GetIO();

        auto tabitem =  ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabItem>(
            "Triangle Demo",
            &m_triangle_demo
        );
        if(!tabitem)
            return;
        ImGui::ColorEdit4("Color of Triangle", &m_triangle_color[0]);
        if(ImGui::BeginChild("##glcanvas", ImVec2(0,0), true))
        {
            auto& io = ImGui::GetIO();
            auto* window = ImGui::GetCurrentWindow();
            ImVec2 pos = window->Pos;
            ImVec2 size = window->InnerRect.GetSize();
            m_triangle_viewport = glm::vec4{ pos.x, pos.y+size.y , size.x, size.y };

            auto* list = ImGui::GetWindowDrawList();
            list->AddCallback(
                [](const ImDrawList* parent, const ImDrawCmd* cmd)
                {
                    auto* this_ = (OpenGL3DemoWindow*)cmd->UserCallbackData;

                    if(detail::SetupScissorsAndViewport(ImGui::GetDrawData(), cmd->ClipRect, this_->m_triangle_viewport))
                    {
                        GLint last_vao; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vao);
                        GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
                        glUseProgram(this_->m_triangle_shader);
                        Uniform(this_->m_triangle_shader.UniformLocation("unicolor"), this_->m_triangle_color);
                        glBindVertexArray(this_->m_triangle_vao);
                        glDrawArrays(GL_TRIANGLES, 0, 3);
                        glBindVertexArray(last_vao);
                        glUseProgram(last_program);
                    }
                },
                this
            );
            list->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
        }
        ImGui::EndChild();
    }
    void OpenGL3DemoWindow::TextureDemoTabItem()
    {
        auto& io = ImGui::GetIO();

        auto tabitem = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabItem>(
            "Texture Demo",
            &m_texture_demo
        );
        if(!tabitem)
            return;

        auto& desc = m_texture_desc;
        ImGui::Text("Wrapping: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f);
        m_texture_descchanged |= ImGui::Combo("S", (int*)&desc.s, "Repeat\0Mirrored Repeat\0Clamp to Edge\0Clamp to Border\0");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f);
        m_texture_descchanged |= ImGui::Combo("T", (int*)&desc.t, "Repeat\0Mirrored Repeat\0Clamp to Edge\0Clamp to Border\0");
        if(desc.s == Texture::CLAMP_TO_BORDER || desc.t == Texture::CLAMP_TO_BORDER) // Clamp to Border
        {
            ImGui::SetNextItemWidth(400.0f);
            m_texture_descchanged |= ImGui::ColorEdit4("Border", &m_texture_desc.border_color[0]);
        }
        ImGui::Text("Filter:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f);
        m_texture_descchanged |= ImGui::Combo("Min", (int*)&desc.min, "Linear\0Nearest\0");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f); 
        m_texture_descchanged |= ImGui::Combo("Mag", (int*)&desc.mag, "Linear\0Nearest\0");
        ImGui::Separator();

        if(!tabitem)
            return;
        ImGui::InputText(
            "##location",
            m_texture_location_buffer.data(),
            m_texture_location_buffer.size()
        );
        ImGui::SameLine();
        if(ImGui::Button("Open"))
        {
            m_texture_location = m_texture_location_buffer;
            m_texture_descchanged = false;
            m_texture_texture.LoadFromFileEx(filesystem::u8path(m_texture_location_buffer), m_texture_desc);
            m_texture_uvs = { 0.0f, 0.0f, 1.0f, 1.0f };
        }
        ImGui::SameLine();
        if(ImGui::Button(m_texture_descchanged ? "Reload*###reload" : "Reload###reload"))
        {
            m_texture_descchanged = false;
            if(m_texture_location.empty())
                m_texture_texture.LoadDefaultTexture();
            else
                m_texture_texture.LoadFromFileEx(filesystem::u8path(m_texture_location), m_texture_desc);
        }
        ImGui::SameLine();
        if(ImGui::Button("Clear"))
        {
            m_texture_location.clear();
            m_texture_desc = Texture::Description();
            m_texture_descchanged = false;
            m_texture_texture.LoadDefaultTexture();
            m_texture_uvs = { 0.0f, 0.0f, 1.0f, 1.0f };
        }
        ImGui::SetNextItemWidth(200.0f);
        ImGui::SliderFloat2("UV 0", &m_texture_uvs[0], -2.0f, 2.0f, "%.02f", 0.01f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f);
        ImGui::SliderFloat2("UV 1", &m_texture_uvs[2], -2.0f, 2.0f, "%.02f", 0.01f);
        ImGui::SameLine();
        if(ImGui::Button("Reset"))
        {
            m_texture_uvs = { 0.0f, 0.0f, 1.0f, 1.0f };
        }
        ImGui::Separator();
        if(ImGui::BeginChild("##texture", ImVec2(0, 0)))
        {
            ImGui::Image(
                m_texture_texture.GetNativeHandle(),
                ImGui::GetWindowSize(),
                ImVec2(m_texture_uvs.x, m_texture_uvs.y),
                ImVec2(m_texture_uvs.z, m_texture_uvs.w)
            );
        }
        ImGui::EndChild();
    }

    void OpenGL3DemoWindow::EffectDemoTabItem()
    {
        auto tabitem = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabItem>(
            "Effect Demo",
            &m_effect_demo
        );
        if(!tabitem)
            return;
        if(m_texture_location.empty())
        {
            ImGui::TextDisabled("(Please open an image in the Texture Demo)");
        }
        else
        {
            ImGui::Text("Texture: %s", m_texture_location.c_str());
        }

        int selected_effect = static_cast<int>(m_effect_id);
        ImGui::SetNextItemWidth(200.0f);
        ImGui::Combo("Effect", &selected_effect, "Normal\0Inversion\0Grayscale\0Sharpen\0Blur\0");
        m_effect_id = selected_effect;

        ImGui::SameLine();
        ImGui::TextDisabled("(Use \"Clamp to Edge\" as wrapping method for better result)");

        if(ImGui::BeginChild("##effectcanvas"))
        {
            auto& io = ImGui::GetIO();
            auto* window = ImGui::GetCurrentWindow();
            ImVec2 pos = window->Pos;
            ImVec2 size = window->InnerRect.GetSize();
            m_effect_viewport = glm::vec4{ pos.x, pos.y+size.y , size.x, size.y };

            auto* list = ImGui::GetWindowDrawList();
            list->AddCallback(
                [](const ImDrawList* parent, const ImDrawCmd* cmd)
                {
                    auto* this_ = (OpenGL3DemoWindow*)cmd->UserCallbackData;

                    if(detail::SetupScissorsAndViewport(ImGui::GetDrawData(), cmd->ClipRect, this_->m_effect_viewport))
                    {
                        GLint last_vao; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vao);
                        GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);

                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, this_->m_texture_texture.handle());

                        auto& sh = this_->m_effect_shaders[this_->m_effect_id];
                        glUseProgram(sh);
                        glBindVertexArray(this_->m_effect_vao);
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        glBindVertexArray(last_vao);
                        glUseProgram(last_program);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    }
                },
                this
            );
            list->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
        }
        ImGui::EndChild();
    }
} // namespace srose::gpu::opengl3

#endif
