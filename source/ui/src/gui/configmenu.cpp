/**
 * @file configmenu.cpp
 * @author HenryAWE
 * @brief Configuration panel
 */

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "configmenu.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imguisr.h>
#include <sr/ui/uimgr.hpp>
#include <sr/gpu/renderer.hpp>
#include <sr/audio/aumgr.hpp>
#include <sr/player/player.hpp>
#include <sr/i18n/i18n.hpp>
#include "conwin.hpp"


namespace srose::ui
{
    ConfigPanel::ConfigPanel(wm::Window& window)
        : Base("configpanel"), m_window(&window)
    {
        LoadButtons();
        m_conwin = std::make_shared<ConsoleWindow>("conwin");
    }

    void ConfigPanel::Update()
    {
        Base::Update();

        auto& io = ImGui::GetIO();

        SetFlags(ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        auto background = BeginContext();

        /* Configuration items' list */
        ImVec2 configitems_region(ImGui::GetWindowContentRegionWidth() * 0.18f, io.DisplaySize.y * 0.7f);
        ImGui::BeginChild("##configitems", configitems_region, true, ImGuiWindowFlags_NoScrollbar);
        using std::get;
        const float button_width = ImGui::GetWindowContentRegionWidth();
        for(const auto& i : m_buttons)
        {
            if(ImGui::Button(get<0>(i).c_str(), ImVec2(button_width, 0.0f)))
            {
                auto cb = get<1>(i);
                if(cb) (this->*cb)();
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        /* Item content */
        ImVec2 content_region(ImGui::GetContentRegionAvail().x * 0.85f, io.DisplaySize.y * 0.7f);
        ImGui::BeginChild("##content", content_region, true);
        if(m_content_func)
        {
            (this->*m_content_func)();
        }
        ImGui::EndChild();

#ifndef SROSE_DISABLE_DEMO
        if(m_show_player_demo)
            player::ShowDemoWindow(*m_window, &m_show_player_demo);
        if(m_show_render_demo)
            m_window->GetRenderer().ShowDemoWindow(&m_show_render_demo);
        if(m_show_audio_demo)
            audio::GetAudioManager()->ShowDemoWindow(&m_show_audio_demo);

        player::RenderDemoWindow();
#endif
    }

    void ConfigPanel::LoadButtons()
    {
        using std::make_pair;

        constexpr int BUTTON_COUNT = 3;
        m_buttons.reserve(BUTTON_COUNT);
        m_buttons.push_back(make_pair(gettext("srose.ui.configpanel.video") + "###video", &ConfigPanel::Button_Video));
        m_buttons.push_back(make_pair(gettext("srose.ui.configpanel.lang") + "###lang", &ConfigPanel::Button_Language));
        m_buttons.push_back(make_pair(gettext("srose.ui.configpanel.developer") + "###developer", &ConfigPanel::Button_Developer));
        m_buttons.push_back(make_pair(gettext("srose.ui.configpanel.return") + "###return", &ConfigPanel::Button_Return));

        AddString("windowed", "srose.ui.configpanel.video.windowed");
        AddString("show-conwin", "srose.ui.configpanel.developer.show-conwin");
    }
    void ConfigPanel::ResetStates()
    {
        m_content_func = nullptr;
    }

    void ConfigPanel::LoadI18nData()
    {
        Base::LoadI18nData();

        m_buttons[0].first = gettext("srose.ui.configpanel.video") + "###video";
        m_buttons[1].first = gettext("srose.ui.configpanel.lang") + "###lang";
        m_buttons[2].first = gettext("srose.ui.configpanel.developer") + "###developer";
        m_buttons[3].first = gettext("srose.ui.configpanel.return") + "###return";
    }

    void ConfigPanel::Button_Video()
    {
        if(m_content_func != &ConfigPanel::Content_Video)
        {
            ResetStates();
            m_content_func = &ConfigPanel::Content_Video;
        }
    }
    void ConfigPanel::Button_Language()
    {
        if(m_content_func != &ConfigPanel::Content_Language)
        {
            ResetStates();
            m_content_func = &ConfigPanel::Content_Language;
        }
    }
    void ConfigPanel::Button_Developer()
    {
        if(m_content_func != &ConfigPanel::Content_Developer)
        {
            ResetStates();
            m_content_func = &ConfigPanel::Content_Developer;
        }
    }
    void ConfigPanel::Button_Return()
    {
        ResetStates();
        m_show_conwin = false;
#ifndef SROSE_DISABLE_DEMO
        m_show_player_demo = false;
        m_show_render_demo = false;
        m_show_audio_demo = false;
#endif
        auto& uimgr = UIManager::GetInstance();
        uimgr.PopRootNode();
    }

    void ConfigPanel::Content_Video()
    {
        SDL_Window* win = m_window->handle();
        bool windowed = !(SDL_GetWindowFlags(win)&SDL_WINDOW_FULLSCREEN);
        ImGui::Checkbox(GetString("windowed").c_str(), &windowed);
    }
    void ConfigPanel::Content_Language()
    {
        auto& langs = i18n::GetLanguageMap();
        bool first = true;
        for(auto&[id, lang] : langs)
        {
            ImGui::PushID(id.c_str());
            if(!first)
                ImGui::SameLine();
            else
                first = false;
            if(ImGui::Button(lang->name().c_str()))
            {
                i18n::SelectLanguage(lang);
                UIManager::GetInstance().imbue(std::locale());
            }

            ImGui::PopID();
        }
        ImGui::Separator();
        ImGui::Text(i18n::GetDefaultLanguage()->name().c_str());
        ImGui::SameLine();
        ImGui::TextDisabled("(%s)", i18n::GetDefaultLanguage()->iso().c_str());
    }
    void ConfigPanel::Content_Developer()
    {
        auto& uimgr = UIManager::GetInstance();
        if(ImGui::Button(GetString("show-conwin").c_str()))
        {
            auto& nodes = uimgr.GetStandaloneNodes();
            auto iter = std::find_if(
                nodes.begin(), nodes.end(),
                [](auto& v){ return v->GetName() == "conwin"; }
            );
            if(iter == nodes.end())
            {
                nodes.push_back(m_conwin);
            }
        }
#ifndef SROSE_DISABLE_DEMO
        ImGui::Separator();
        ImGui::Checkbox("Player test", &m_show_player_demo);
        ImGui::SameLine();
        ImGui::Checkbox("Rendering test", &m_show_render_demo);
        ImGui::SameLine();
        ImGui::Checkbox("Audio test", &m_show_audio_demo);
#endif
    }
} // namespace srose::ui
