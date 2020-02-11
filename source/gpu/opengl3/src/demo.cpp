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


namespace srose::gpu::opengl3
{
    OpenGL3DemoWindow::OpenGL3DemoWindow(bool initgl)
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

        m_gl_initialized = true;
    }
    void OpenGL3DemoWindow::ReleaseGL() noexcept
    {
        m_gl_initialized = false;
        m_triangle_vao.Destroy();
        m_triangle_vbo.Destroy();
        m_triangle_shader.Destroy();

        m_texture_texture.Destroy();
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
            m_triangle_demo ||
            m_texture_demo;
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
                ImGui::Checkbox("Triangle Demo", &m_triangle_demo);
                ImGui::Checkbox("Texture Demo", &m_texture_demo);
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
            if(m_triangle_demo) TriangleDemoTabItem();
            if(m_texture_demo) TextureDemoTabItem();
        }
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
                    auto* draw_data = ImGui::GetDrawData();
                    ImVec2 clip_off = draw_data->DisplayPos;
                    ImVec2 clip_scale = draw_data->FramebufferScale;
                    ImVec4 clip_rect;
                    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
                    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
                    if (fb_width <= 0 || fb_height <= 0)
                        return;
                    clip_rect.x = (cmd->ClipRect.x - clip_off.x) * clip_scale.x;
                    clip_rect.y = (cmd->ClipRect.y - clip_off.y) * clip_scale.y;
                    clip_rect.z = (cmd->ClipRect.z - clip_off.x) * clip_scale.x;
                    clip_rect.w = (cmd->ClipRect.w - clip_off.y) * clip_scale.y;
                    if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                    {
                        GLint last_vao; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vao);
                        GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
                        glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
                        auto viewport = this_->m_triangle_viewport;;
                        glViewport(viewport.x, fb_height - viewport.y, viewport.z, viewport.w);
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
        ImGui::InputText(
            "##location",
            m_texture_location_buffer.data(),
            m_texture_location_buffer.size()
        );
        ImGui::SameLine();
        if(ImGui::Button("Open"))
        {
            m_texture_location = m_texture_location_buffer;
            m_texture_texture.LoadFromFile(filesystem::u8path(m_texture_location_buffer));
            m_texture_uvs = { 0.0f, 0.0f, 1.0f, 1.0f };
        }
        ImGui::SameLine();
        if(ImGui::Button("Reload"))
        {
            if(m_texture_location.empty())
                m_texture_texture.LoadDefaultTexture();
            else
                m_texture_texture.LoadFromFile(filesystem::u8path(m_texture_location));
            m_texture_uvs = { 0.0f, 0.0f, 1.0f, 1.0f };
        }
        ImGui::SameLine();
        if(ImGui::Button("Clear"))
        {
            m_texture_location.clear();
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
} // namespace srose::gpu::opengl3
