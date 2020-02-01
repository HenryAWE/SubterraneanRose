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
#include "uimgr.hpp"


namespace srose::ui
{
    MainMenu::MainMenu()
    {
        using std::make_pair;

        constexpr int BUTTON_COUNT = 2;
        m_buttons.reserve(BUTTON_COUNT);
        m_buttons.push_back(make_pair(gettext("srose.ui.mainmenu.config") + "###config", nullptr));
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

    void MainMenu::Button_Exit()
    {
        GetUIManager()->Quit();
    }
} // namespace srose::ui
