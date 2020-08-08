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
#include <sr/ui/uimgr.hpp>
#include <sr/filesystem/filesystem.hpp>
#include <fstream>
#include <sstream>


namespace srose::ui
{
    MainMenu::MainMenu()
        : Base("srose.mainmenu")
    {
        Connect(UIManager::GetInstance().OnImbue);

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
        Base::Update();

        auto& io = ImGui::GetIO();

        SetFlags(ImGuiWindowFlags_NoScrollbar);
        auto background = BeginContext();

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

    void MainMenu::LoadI18nData()
    {
        m_buttons[0].first = gettext("srose.ui.editor") + "###editor";
        m_buttons[1].first = gettext("srose.ui.mainmenu.config") + "###config";
        m_buttons[2].first = gettext("srose.ui.about") + "###about";
        m_buttons[3].first = gettext("srose.ui.mainmenu.exit") + "###exit";
    }

    void MainMenu::Button_Editor()
    {
        auto& uimgr = UIManager::GetInstance();
        uimgr.PushRootNode(uimgr.GetUINodeTree()["editor"]);
    }
    void MainMenu::Button_Config()
    {
        auto& uimgr = UIManager::GetInstance();
        uimgr.PushRootNode(uimgr.GetUINodeTree()["configpanel"]);
    }
    void MainMenu::Button_About()
    {
        auto& uimgr = UIManager::GetInstance();
        uimgr.PushRootNode(uimgr.GetUINodeTree()["about"]);
    }
    void MainMenu::Button_Exit()
    {
        UIManager::GetInstance().Quit();
    }
} // namespace srose::ui
