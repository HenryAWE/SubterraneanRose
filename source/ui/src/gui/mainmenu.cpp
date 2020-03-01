/**
 * @file mainmenu.cpp
 * @author HenryAWE
 * @brief Main Menu
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "mainmenu.hpp"
#include <cassert>
#include <imgui_internal.h>
#include <sr/ui/gui/uimgr.hpp>
#include "editor/editor_window.hpp"


namespace srose::ui
{
    MainMenu::MainMenu()
    {
        using std::make_pair;

        constexpr int BUTTON_COUNT = 4;
        m_buttons.reserve(BUTTON_COUNT);
        m_buttons.push_back(make_pair(gettext("srose.ui.editor") + "###editor", &MainMenu::Button_Editor));
        m_buttons.push_back(make_pair(gettext("srose.ui.mainmenu.config") + "###config", &MainMenu::Button_Config));
        m_buttons.push_back(make_pair(gettext("srose.ui.about") + "###about", &MainMenu::Button_About));
        m_buttons.push_back(make_pair(gettext("srose.ui.mainmenu.exit") + "###exit", &MainMenu::Button_Exit));
    }

    void MainMenu::Update()
    {
        Widget::Update();

        auto& io = ImGui::GetIO();

        int background_flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings;
        ImGui::SetNextWindowPosCenter();
        ImGui::SetNextWindowSize(io.DisplaySize/2.0f);
        auto background = ImGuiSR::PushGuard<ImGuiSR::ImGuiSR_Window>(
            "##srose.mainmenu",
            nullptr,
            background_flags
        );
        assert(background);

        using std::get;
        const float button_height = (io.DisplaySize.y) / static_cast<float>(2 * m_buttons.size());
        for(const auto& i : m_buttons)
        {
            if(ImGui::Button(get<0>(i).c_str(), ImVec2(-1.0f, button_height)))
            {
                auto cb = get<1>(i);
                if(cb) (this->*cb)();
            }
        }
    }

    void MainMenu::Load()
    {
        m_buttons[0].first = gettext("srose.ui.editor") + "###editor";
        m_buttons[1].first = gettext("srose.ui.mainmenu.config") + "###config";
        m_buttons[2].first = gettext("srose.ui.about") + "###about";
        m_buttons[3].first = gettext("srose.ui.mainmenu.exit") + "###exit";
    }
    void MainMenu::OnImbue()
    {
        Load();
    }

    void MainMenu::Button_Editor()
    {
        auto& uimgr = *GetUIManager();
        uimgr.widget_stack.push(uimgr.widget_tree["editor.window"]);
    }
    void MainMenu::Button_Config()
    {
        auto& uimgr = *GetUIManager();
        uimgr.widget_stack.push(uimgr.widget_tree["configpanel"]);
    }
    void MainMenu::Button_About()
    {
        auto& uimgr = *GetUIManager();
        uimgr.widget_stack.push(uimgr.widget_tree["about"]);
    }
    void MainMenu::Button_Exit()
    {
        GetUIManager()->Quit();
    }
} // namespace srose::ui
