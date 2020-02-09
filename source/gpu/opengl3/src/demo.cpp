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
        m_gl_initialized = true;
    }
    void OpenGL3DemoWindow::ReleaseGL() noexcept
    {
        m_gl_initialized = false;
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
            m_triangle_demo;
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
            }
        }
    }
    void OpenGL3DemoWindow::UpdateTabBar()
    {
        constexpr int tabbar_flags = 
            ImGuiTabBarFlags_NoTooltip |
            ImGuiTabBarFlags_NoTooltip;
        auto tabbar = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabBar>("##tabbar", tabbar_flags);
        if(tabbar)
        {
            if(m_triangle_demo) TriangleDemoTabItem();
        }
    }

    void OpenGL3DemoWindow::TriangleDemoTabItem()
    {
        auto tabitem =  ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_TabItem>(
            "Triangle Demo",
            &m_triangle_demo
        );
        if(!tabitem)
            return;

    }
} // namespace srose::gpu::opengl3
