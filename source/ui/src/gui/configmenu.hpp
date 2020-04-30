/**
 * @file configmenu.hpp
 * @author HenryAWE
 * @brief Configuration panel
 */

#ifndef SROSE_UI_GUI_configmenu_hpp_
#define SROSE_UI_GUI_configmenu_hpp_

#include <vector>
#include <utility>
#include <sr/ui/widget.hpp>


namespace srose::ui
{
    class ConfigPanel : public Widget
    {
    public:
        ConfigPanel();

        void Update() override;

    private:
        void LoadButtons();
        void ResetStates();

        void OnImbue() override;

        using callback_type = void(ConfigPanel::*)();
        std::vector<std::pair<std::string, callback_type>> m_buttons;

        void Button_Video();
        void Button_Language();
        void Button_Developer();
        void Button_Return();

        callback_type m_content_func = nullptr;

        void Content_Video();
        bool m_show_conwin = false;
#ifndef SROSE_DISABLE_DEMO
        bool m_show_player_demo = false;
        bool m_show_render_demo = false;
        bool m_show_audio_demo = false;
#endif
        void Content_Developer();
        void Content_Language();
    };
} // namespace srose::ui


#endif
