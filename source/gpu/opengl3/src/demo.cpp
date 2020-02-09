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

        constexpr int background_flag = 
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_MenuBar;
        ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_Appearing);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            "OpenGL3 Demo",
            &open,
            background_flag
        );
        if(!background_flag)
            return;
    }
} // namespace srose::gpu::opengl3
