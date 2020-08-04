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

        m_crash_report_title = gettext("srose.ui.crash-report.title") + "###crash-report";
        m_crash_report_info = gettext("srose.ui.crash-report.info");
        namespace fs = filesystem;
        fs::path dump_file = "sr-dump.txt";
        if(fs::exists(dump_file))
        {
            std::ifstream dump(dump_file);
            std::stringstream ss;
            ss << dump.rdbuf();
            dump.close();
            m_crash_report_stacktrace = ss.str();
            fs::remove(dump_file);
        }
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

        if(!m_crash_report_stacktrace.empty())
        {
            ImGui::OpenPopup(m_crash_report_title.c_str());
            bool crash_report_open = true;

            constexpr int popup_flags =
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoMove;

            ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.75f, 0), ImGuiCond_Always);
            if(ImGui::BeginPopupModal(m_crash_report_title.c_str(), &crash_report_open, popup_flags))
            {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), m_crash_report_info.c_str());
                ImGui::Checkbox("Stacktrace", &m_show_stacktrace);
                if(m_show_stacktrace)
                {
                    ImGui::BeginChildFrame(ImGui::GetID("traceinfo"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 18), ImGuiWindowFlags_HorizontalScrollbar);
                    ImGui::TextUnformatted(
                        m_crash_report_stacktrace.c_str(),
                        m_crash_report_stacktrace.c_str() + m_crash_report_stacktrace.length()
                    );
                    ImGui::EndChildFrame();
                }
                ImGui::EndPopup();
            }
            if(crash_report_open == false)
            {
                ImGui::CloseCurrentPopup();
                std::string().swap(m_crash_report_stacktrace);
            }
        }
    }

    void MainMenu::LoadI18nData()
    {
        m_buttons[0].first = gettext("srose.ui.editor") + "###editor";
        m_buttons[1].first = gettext("srose.ui.mainmenu.config") + "###config";
        m_buttons[2].first = gettext("srose.ui.about") + "###about";
        m_buttons[3].first = gettext("srose.ui.mainmenu.exit") + "###exit";

        m_crash_report_title = gettext("srose.ui.crash-report.title") + "###crash-report";
        m_crash_report_info = gettext("srose.ui.crash-report.info");
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
