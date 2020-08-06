/**
 * @file state.cpp
 * @author HenryAWE
 * @brief Editor state machine
 */

#include <imgui.h>
#include "state.hpp"
#include <fstream>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>
#include <imguisr.h>
#include <sr/trace/log.hpp>
#include "impl.hpp"


namespace srose::ui::editor
{
    StateWelcomeWindow::StateWelcomeWindow() = default;

    StateWelcomeWindow::~StateWelcomeWindow() = default;

    sc::result StateWelcomeWindow::react(const EventUpdate&)
    {
        auto id = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_ID>("srose.editor.welcome");

        auto& io = ImGui::GetIO();

        ImGui::SetNextWindowPosCenter();
        ImGui::SetNextWindowSize(io.DisplaySize * 0.8f, ImGuiCond_Appearing);
        constexpr int flags =
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove;
        bool open = true;
        auto window = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>("Welcome", &open, flags);
        if(!window)
            return discard_event();
        if(ImGui::Button("Open"))
        {
            post_event(EventLoadProject(EventLoadProject::LOAD_OPEN_FILE));
        }
        if(ImGui::Button("New"))
        {
            post_event(EventLoadProject(EventLoadProject::LOAD_NEW_INSTANCE));
        }

        ImGui::Separator();

        if(ImGui::Button("Quit"))
        {
            post_event(EventQuit{});
        }

        if(!open)
            return transit<StateIdle>();
        
        return discard_event();
    }
    sc::result StateWelcomeWindow::react(const EventLoadProject& e)
    {
        auto& editor = outermost_context().editor;
        auto& ifile = editor.GetEditorImpl().GetFileOpenDialog();

        switch(e.from)
        {
        case EventLoadProject::LOAD_OPEN_FILE:
            ifile.SetTitle("Open Project");
            ifile.SetCallback(
                [this, &editor](ImGuiSR::IFileBrowser& ifile)
                {
                    auto result = ifile.GetResult();
                    if(result)
                    {
                        std::ifstream ifs(*result);
                        editor.OpenProject(ifs);
                    }
                }
            );
            ifile.Show();
            return discard_event();

        case EventLoadProject::LOAD_NEW_INSTANCE:
            editor.NewProject();
            break;
        }

        return transit<StateIdle>();
    }

    StateIdle::StateIdle()
    {
        BOOST_LOG_TRIVIAL(info) << "[UI] Enter StateIdle";
    }

    StateIdle::~StateIdle()
    {
        BOOST_LOG_TRIVIAL(info) << "[UI] Leave StateIdle";
    }
} // namespace srose::ui::editor
