/**
 * @file welcome.cpp
 * @author HenryAWE
 * @brief Welcome window
 */


#include <imgui.h>
#include "welcome.hpp"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>
#include "editor_state.hpp"


namespace srose::ui::editor
{
    WelcomeWindow::WelcomeWindow()
        : Base("Welcome", "srose.editor.welcome")
    {

    }

    WelcomeWindow::~WelcomeWindow() = default;

    void WelcomeWindow::Update()
    {
        if(!visible())
        {
            
        }

        auto& io = ImGui::GetIO();
        ImGui::SetNextWindowPosCenter();
        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_Appearing);
        SetFlags(ImGuiWindowFlags_NoMove);
        auto window = BeginContext();
        if(!window)
            return;
    }

    void WelcomeWindow::LoadI18nData()
    {

    }
} // namespace srose::ui::editor
